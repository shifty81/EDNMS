# EDNMS Technical Specifications
## Custom Engine Implementation Guide

---

## 1. System Architecture

### 1.1 Overall Architecture
```
┌─────────────────────────────────────────────────────────────┐
│                        Client Layer                          │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐   │
│  │   UI     │  │ Renderer │  │  Audio   │  │  Input   │   │
│  └──────────┘  └──────────┘  └──────────┘  └──────────┘   │
└─────────────────────────────────────────────────────────────┘
                            ↓↑
┌─────────────────────────────────────────────────────────────┐
│                      Game Logic Layer                        │
│  ┌────────────┐  ┌────────────┐  ┌────────────┐           │
│  │  Flight    │  │  Building  │  │  Survival  │           │
│  │  System    │  │  System    │  │  System    │           │
│  └────────────┘  └────────────┘  └────────────┘           │
│  ┌────────────┐  ┌────────────┐  ┌────────────┐           │
│  │  Combat    │  │  Economy   │  │  Faction   │           │
│  │  System    │  │  System    │  │  System    │           │
│  └────────────┘  └────────────┘  └────────────┘           │
└─────────────────────────────────────────────────────────────┘
                            ↓↑
┌─────────────────────────────────────────────────────────────┐
│                   Procedural Generation Layer                │
│  ┌────────────┐  ┌────────────┐  ┌────────────┐           │
│  │  Galaxy    │  │  Planet    │  │  Creature  │           │
│  │  Generator │  │  Generator │  │  Generator │           │
│  └────────────┘  └────────────┘  └────────────┘           │
└─────────────────────────────────────────────────────────────┘
                            ↓↑
┌─────────────────────────────────────────────────────────────┐
│                       Network Layer                          │
│  ┌────────────┐  ┌────────────┐  ┌────────────┐           │
│  │  Session   │  │  P2P       │  │  Server    │           │
│  │  Manager   │  │  Network   │  │  Sync      │           │
│  └────────────┘  └────────────┘  └────────────┘           │
└─────────────────────────────────────────────────────────────┘
                            ↓↑
┌─────────────────────────────────────────────────────────────┐
│                      Persistence Layer                       │
│  ┌────────────┐  ┌────────────┐  ┌────────────┐           │
│  │  Save      │  │  Cloud     │  │  Discovery │           │
│  │  System    │  │  Sync      │  │  Database  │           │
│  └────────────┘  └────────────┘  └────────────┘           │
└─────────────────────────────────────────────────────────────┘
```

### 1.2 Game Engine
**Decision: Custom Simulation-First Engine**

After evaluating commercial engines, the project will use a custom engine designed specifically for this game's simulation requirements. See [ENGINE_ARCHITECTURE.md](ENGINE_ARCHITECTURE.md) for full details.

- **Core Architecture**: ECS-based with engine-agnostic simulation layer
- **Key Advantages**:
  - Full control over simulation fidelity and chunk streaming
  - Physical persistence guaranteed at the engine level
  - Double-precision math for space-scale coordinates
  - Deterministic simulation enabling headless testing
  - No dependency on third-party engine roadmaps
- **Rendering**: Initially minimal (OpenGL/Vulkan), upgraded over time
- **Language**: C++17, data-driven design, no scripting until systems stabilize
- **Build System**: CMake with static libraries per layer

**Why Not Unreal Engine 5?**
While Unreal offers excellent rendering (Nanite, Lumen) and World Partition, the simulation-first approach requires engine-agnostic logic. The hybrid path keeps all gameplay logic independent, allowing a potential renderer swap later.

---

## 2. Core Module Specifications

### 2.1 Flight System Module

#### Ship Physics Engine
```cpp
class ShipPhysics {
    // 6DOF Movement
    Vector3 position;
    Quaternion rotation;
    Vector3 velocity;
    Vector3 angularVelocity;
    
    // Ship Properties
    float mass;
    float thrust_forward;
    float thrust_lateral;
    float thrust_vertical;
    float rotational_thrust;
    float maxSpeed;
    
    // Flight Assist
    bool flightAssist;
    
    void Update(float deltaTime);
    void ApplyThrust(Vector3 direction, float power);
    void ApplyRotation(Vector3 axis, float power);
    void CalculateFlightAssist();
};
```

#### Power Management
```cpp
class PowerDistributor {
    float totalPower = 100.0f;
    float systemsPower = 33.3f;  // Shields
    float enginesPower = 33.3f;  // Speed/Boost
    float weaponsPower = 33.3f;  // Weapons
    
    void SetDistribution(float sys, float eng, float wep);
    float GetSystemCapacity();
    float GetEngineBoost();
    float GetWeaponDamage();
};
```

#### Ship Hardpoint System
```cpp
enum WeaponMount {
    FIXED,      // Direct aim, high damage
    GIMBALLED,  // Auto-tracking, medium damage
    TURRETED    // 360° coverage, low damage
};

class Hardpoint {
    WeaponMount mountType;
    Weapon* equippedWeapon;
    Transform hardpointTransform;
    bool isRetracted;
    
    void Fire();
    void ToggleDeploy();
};
```

### 2.2 Procedural Generation Module

#### Galaxy Generation
```cpp
class GalaxyGenerator {
    uint64_t galaxySeed;
    
    struct StarSystem {
        uint64_t systemId;
        Vector3 position;
        string starClass;  // O, B, A, F, G, K, M
        int planetCount;
        vector<Planet> planets;
    };
    
    StarSystem GenerateSystem(uint64_t systemId);
    vector<StarSystem> GetNearbySystems(Vector3 position, float radius);
};
```

#### Planet Generation
```cpp
class PlanetGenerator {
    struct BiomeType {
        string name;
        Color primaryColor;
        Color secondaryColor;
        float temperature;
        float humidity;
        TerrainProfile terrain;
    };
    
    struct Planet {
        uint64_t planetId;
        float radius;
        float gravity;
        BiomeType biome;
        bool hasAtmosphere;
        bool hasWater;
        float rotationPeriod;
        
        // Resources
        map<string, float> resourceDeposits;
    };
    
    Planet GeneratePlanet(uint64_t seed, int index);
    TerrainData GenerateTerrainChunk(Vector3 position, int LOD);
};
```

#### Creature Generation
```cpp
class CreatureGenerator {
    struct Creature {
        string speciesId;
        MeshData bodyMesh;
        float size;
        float speed;
        bool isAggressive;
        Color[] colorScheme;
        
        // Behavior
        AIBehavior behavior;
        float detectionRange;
        string soundProfile;
    };
    
    Creature GenerateCreature(uint64_t seed, BiomeType biome);
};
```

### 2.3 Building System Module

#### Base Building
```cpp
class BuildingSystem {
    enum StructureType {
        FOUNDATION,
        WALL,
        FLOOR,
        ROOF,
        DOOR,
        WINDOW,
        STORAGE,
        REFINERY,
        POWER_GEN,
        TELEPORTER
    };
    
    struct BuildingPiece {
        StructureType type;
        Transform transform;
        vector<Resource> buildCost;
        bool isPlaced;
        float integrity;
    };
    
    bool CanPlacePiece(BuildingPiece piece, Vector3 position);
    void PlacePiece(BuildingPiece piece);
    void RemovePiece(BuildingPiece piece);
    bool CheckStructuralIntegrity(BaseStructure base);
};
```

#### Station Construction
```cpp
class StationBuilder {
    enum ModuleType {
        CORE_HUB,
        HABITAT_RING,
        DOCKING_BAY,
        MANUFACTURING,
        AGRICULTURE,
        DEFENSE,
        STORAGE,
        REFINERY,
        RESEARCH_LAB
    };
    
    struct StationModule {
        ModuleType type;
        Vector3 attachmentPoint;
        Quaternion orientation;
        float powerConsumption;
        float crewRequirement;
        bool isOperational;
    };
    
    class SpaceStation {
        vector<StationModule> modules;
        float totalPower;
        int totalCrew;
        Economy stationEconomy;
        
        void AddModule(StationModule module);
        void UpdateStation(float deltaTime);
        void ProcessTrade(TradeTransaction transaction);
    };
};
```

### 2.4 Survival System Module

#### Life Support
```cpp
class LifeSupport {
    // Resource meters
    float oxygen = 100.0f;
    float health = 100.0f;
    float temperature = 37.0f;  // Celsius
    float hunger = 100.0f;
    float thirst = 100.0f;
    float fatigue = 100.0f;
    
    // Suit protection
    float hazardProtection = 100.0f;
    float thermalProtection = 100.0f;
    float radiationProtection = 100.0f;
    
    void Update(float deltaTime, Environment env);
    void ApplyDamage(float amount, DamageType type);
    void Consume(ConsumableItem item);
    void Rest(float duration);
};
```

#### Environmental Hazards
```cpp
class Environment {
    enum HazardType {
        NONE,
        EXTREME_HEAT,
        EXTREME_COLD,
        TOXIC,
        RADIOACTIVE,
        UNDERWATER
    };
    
    HazardType currentHazard;
    float hazardIntensity;  // 0-100
    float temperature;
    float atmosphericPressure;
    bool hasOxygen;
    
    float CalculateDamagePerSecond(LifeSupport suit);
};
```

### 2.5 Combat System Module

#### Weapon System
```cpp
class Weapon {
    enum WeaponType {
        PULSE_LASER,
        BEAM_LASER,
        CANNON,
        MISSILE,
        RAILGUN,
        PLASMA_ACCELERATOR
    };
    
    WeaponType type;
    float damage;
    float fireRate;
    float range;
    float powerDraw;
    float heatGeneration;
    int ammoCount;
    
    virtual void Fire(Transform origin, Target target);
    virtual void Reload();
};
```

#### Shield System
```cpp
class ShieldGenerator {
    float maxShield;
    float currentShield;
    float regenRate;
    float regenDelay;
    bool isBroken;
    
    void TakeDamage(float damage);
    void Update(float deltaTime);
    void Recharge(float amount);
};
```

### 2.6 Economy System Module

#### Market System
```cpp
class Market {
    struct Commodity {
        string name;
        float basePrice;
        float currentPrice;
        int supply;
        int demand;
    };
    
    map<string, Commodity> commodities;
    
    void UpdatePrices(float deltaTime);
    void ProcessTrade(string commodityId, int quantity, bool isBuy);
    float CalculatePrice(Commodity commodity, int quantity);
};
```

#### Player Economy
```cpp
class PlayerCredits {
    long credits;
    
    bool CanAfford(long cost);
    void AddCredits(long amount);
    void SpendCredits(long amount);
    
    // Transaction history
    vector<Transaction> history;
    void RecordTransaction(Transaction t);
};
```

---

## 3. Data Structures

### 3.1 Save Game Format
```json
{
    "version": "1.0.0",
    "playerId": "uuid-string",
    "playerName": "CommanderName",
    "credits": 1000000,
    "playtime": 3600,
    "lastSave": "2026-02-12T00:00:00Z",
    
    "position": {
        "systemId": 123456789,
        "x": 0.0,
        "y": 0.0,
        "z": 0.0
    },
    
    "ship": {
        "model": "CobraMk3",
        "hull": 100.0,
        "modules": [],
        "cargo": []
    },
    
    "inventory": {
        "suit": [],
        "materials": {}
    },
    
    "bases": [],
    "stations": [],
    "discoveries": [],
    "reputation": {}
}
```

### 3.2 Planet Data Format
```json
{
    "planetId": 987654321,
    "seed": 123456789,
    "name": "Procedural Planet Alpha",
    "radius": 6371.0,
    "gravity": 9.81,
    "biome": "Lush",
    "atmosphere": {
        "hasAtmosphere": true,
        "breathable": true,
        "color": [0.5, 0.7, 1.0]
    },
    "temperature": {
        "min": -20,
        "max": 35,
        "average": 15
    },
    "resources": {
        "Iron": 0.8,
        "Carbon": 0.6,
        "Uranium": 0.1
    },
    "discoveries": {
        "flora": [],
        "fauna": []
    }
}
```

---

## 4. Performance Optimization Strategies

### 4.1 Level of Detail (LOD) System
```cpp
class LODManager {
    enum LODLevel {
        LOD_0,  // Highest detail (< 100m)
        LOD_1,  // High detail (100m - 500m)
        LOD_2,  // Medium detail (500m - 2km)
        LOD_3,  // Low detail (2km - 10km)
        LOD_4   // Lowest detail (> 10km)
    };
    
    LODLevel CalculateLOD(float distance);
    void UpdateLODs(Vector3 playerPosition);
};
```

### 4.2 Streaming System
```cpp
class StreamingManager {
    // Async loading
    void LoadChunkAsync(Vector3 position);
    void UnloadChunk(Vector3 position);
    
    // Priority queue
    priority_queue<LoadRequest> loadQueue;
    
    void Update();
    bool IsChunkLoaded(Vector3 position);
};
```

### 4.3 Instancing
```cpp
class InstancedRenderer {
    // GPU Instancing for repeated objects
    void DrawInstanced(Mesh mesh, Material mat, 
                      vector<Matrix4x4> transforms);
    
    // Examples: asteroids, trees, rocks
    void RenderAsteroidField(vector<Asteroid> asteroids);
    void RenderForest(vector<Tree> trees);
};
```

---

## 5. Networking Architecture

### 5.1 Session Types
```cpp
enum SessionType {
    SOLO,           // Offline, no other players
    PRIVATE_GROUP,  // Friends only
    PUBLIC          // Open multiplayer
};

class SessionManager {
    SessionType currentSession;
    vector<Player> connectedPlayers;
    
    void CreateSession(SessionType type);
    void JoinSession(string sessionId);
    void SyncPlayerData(Player player);
};
```

### 5.2 Instancing
```cpp
class InstanceManager {
    const int MAX_PLAYERS_PER_INSTANCE = 32;
    
    struct Instance {
        string instanceId;
        Vector3 centerPosition;
        vector<Player> players;
    };
    
    Instance GetOrCreateInstance(Vector3 position);
    void MigratePlayer(Player p, Instance from, Instance to);
};
```

### 5.3 Data Synchronization
```cpp
class NetworkSync {
    // What to sync
    void SyncPosition(Transform t);
    void SyncShipState(Ship s);
    void SyncCombat(CombatEvent e);
    
    // How often
    const float POSITION_SYNC_RATE = 20.0f;  // 20 Hz
    const float STATE_SYNC_RATE = 2.0f;      // 2 Hz
    
    // Interpolation
    void InterpolatePosition(Transform from, Transform to);
};
```

---

## 6. AI Systems

### 6.1 NPC Ship AI
```cpp
class ShipAI {
    enum AIState {
        IDLE,
        PATROL,
        TRADE,
        COMBAT,
        FLEE,
        DOCK
    };
    
    AIState currentState;
    Ship controlledShip;
    Target currentTarget;
    
    void UpdateBehavior(float deltaTime);
    void EnterCombat(Target enemy);
    void CalculateFlightPath(Vector3 destination);
};
```

### 6.2 Creature AI
```cpp
class CreatureAI {
    enum Behavior {
        PASSIVE,
        NEUTRAL,
        AGGRESSIVE,
        FLEE
    };
    
    Behavior behavior;
    float detectionRadius;
    float aggroRadius;
    
    void UpdateBehavior(float deltaTime);
    void OnPlayerDetected(Player p);
    void Attack(Target target);
};
```

---

## 7. UI/UX Systems

### 7.1 HUD Elements
```
┌────────────────────────────────────────────┐
│  [Speed]        [Crosshair]      [Shields] │
│   250m/s           ⊕               |||||||  │
│                                    |||||||  │
│  [Thrust]                         [Hull]   │
│   ▓▓▓▓▓░░░                        ========  │
│                                             │
│  [Target Info]              [System Info]  │
│   Enemy Ship                  Oxygen: 95%  │
│   Hull: 60%                   Temp: 22°C   │
│   Shield: 40%                 Hazard: None │
└────────────────────────────────────────────┘
```

### 7.2 Building Interface
```
┌────────────────────────────────────────────┐
│  Building Mode                             │
│  ┌──────────────┐  ┌──────────────┐       │
│  │  Structures  │  │  Resources   │       │
│  │  - Floor     │  │  Iron: 500   │       │
│  │  - Wall      │  │  Carbon: 300 │       │
│  │  - Door      │  │  Ferrite: 50 │       │
│  └──────────────┘  └──────────────┘       │
│                                             │
│  [Place] [Rotate] [Delete] [Exit]         │
└────────────────────────────────────────────┘
```

---

## 8. Testing Strategy

### 8.1 Unit Tests
```cpp
// Example: Test flight physics
TEST(ShipPhysics, ThrustApplication) {
    ShipPhysics ship;
    ship.mass = 1000.0f;
    ship.thrust_forward = 5000.0f;
    
    ship.ApplyThrust(Vector3(0, 0, 1), 1.0f);
    ship.Update(1.0f);
    
    EXPECT_GT(ship.velocity.z, 0.0f);
}

// Example: Test procedural generation
TEST(PlanetGenerator, ConsistentGeneration) {
    PlanetGenerator gen;
    Planet p1 = gen.GeneratePlanet(12345, 0);
    Planet p2 = gen.GeneratePlanet(12345, 0);
    
    EXPECT_EQ(p1.radius, p2.radius);
    EXPECT_EQ(p1.biome.name, p2.biome.name);
}
```

### 8.2 Integration Tests
- Ship-to-planet landing sequences
- Multiplayer synchronization
- Save/load consistency
- Market economy simulation

### 8.3 Performance Tests
- Frame rate targets: 60 FPS minimum
- Load time targets: < 5 seconds for planet transitions
- Memory usage: < 8GB on medium settings
- Network latency tolerance: < 200ms

---

## 9. Asset Pipeline

### 9.1 3D Assets
```
Source → Import → Process → Optimize → Package
(.fbx)   (Engine)  (LODs)   (Compress)  (.pak)
```

**Ship Models**:
- LOD0: Full detail (< 50m)
- LOD1: Reduced geometry (50m-500m)
- LOD2: Simplified (500m-5km)
- LOD3: Icon representation (> 5km)

**Included Ship OBJ Assets**:

Two modular ship packs are included in the repository root as starter assets:

| Pack | Files | Ship Class |
|------|-------|------------|
| `CruiserBattleshipModulePack.zip` | `core_m.obj`, `spine_m.obj`, `engine_block_m.obj`, `turret_m.obj`, `hangar_m.obj` | Large (cruiser/battleship) |
| `ModularShipModulePack.zip` | `core_s.obj`, `engine_s.obj`, `weapon_s.obj`, `wing_s.obj` | Small (fighter/multi-role) |

**Hardpoint Convention**: OBJ files use `hp_` prefixed sub-objects to define attachment points:
- `hp_engine_N` — Engine mount points
- `hp_weapon_N` — Weapon hardpoint positions
- `hp_wing_L` / `hp_wing_R` — Wing attachment (left/right)
- `hp_spine` — Spine connector for capital ship module chaining

These OBJ modules are designed for the modular ship assembly system where players and the construction system combine modules to build ships.

**Planet Assets**:
- Procedural materials with parameters
- Tileable textures for terrain
- Instanced models for flora

### 9.2 Audio
```
Type          | Format  | Bitrate  | 3D Audio
------------- | ------- | -------- | --------
Music         | OGG     | 192 kbps | No
Voice         | OGG     | 64 kbps  | No
SFX (near)    | WAV     | 44.1 kHz | Yes
SFX (distant) | OGG     | 96 kbps  | Yes
Ambient       | OGG     | 96 kbps  | Yes
```

### 9.3 Textures
```
Type        | Resolution | Format    | Mipmaps
----------- | ---------- | --------- | -------
Albedo      | 2048x2048  | BC1 (DXT1)| Yes
Normal      | 2048x2048  | BC5       | Yes
Roughness   | 1024x1024  | BC4       | Yes
Metallic    | 1024x1024  | BC4       | Yes
AO          | 1024x1024  | BC4       | Yes
```

---

## 10. Development Tools

### 10.1 Debug Tools
```cpp
class DebugConsole {
    // Developer commands
    void TeleportTo(Vector3 position);
    void SpawnShip(string shipModel);
    void GiveCredits(long amount);
    void SetGodMode(bool enabled);
    void ToggleWireframe();
    void ShowColliders();
    void DisplayPerformanceStats();
};
```

### 10.2 Editor Tools
- **Planet Preview**: Generate and preview planets in editor
- **Ship Configurator**: Design and test ship loadouts
- **Base Builder**: Pre-build base templates
- **Mission Editor**: Create custom missions/events

---

## 11. Security Considerations

### 11.1 Anti-Cheat
```cpp
class AntiCheat {
    // Validate client data
    bool ValidatePosition(Vector3 pos);
    bool ValidateCredits(long credits);
    bool ValidateInventory(Inventory inv);
    
    // Detect suspicious behavior
    void MonitorTeleportation();
    void MonitorResourceGeneration();
    void MonitorSpeedHacks();
};
```

### 11.2 Data Encryption
- Save files: AES-256 encryption
- Network traffic: TLS 1.3
- Account credentials: bcrypt hashing
- Cloud saves: End-to-end encryption

---

## 12. Modding Support

### 12.1 Mod API
```cpp
class ModAPI {
    // Safe exposed functions
    void RegisterCustomShip(ShipDefinition ship);
    void RegisterCustomWeapon(WeaponDefinition weapon);
    void AddCustomPlanetBiome(BiomeDefinition biome);
    void RegisterCustomBuilding(BuildingDefinition building);
    
    // Event hooks
    OnPlayerSpawn(callback);
    OnShipDestroyed(callback);
    OnPlanetDiscovered(callback);
};
```

### 12.2 Mod Loading
```
Mods/
├── CustomShips/
│   ├── ship_model.fbx
│   ├── ship_definition.json
│   └── textures/
├── CustomWeapons/
│   └── weapon_definition.json
└── CustomBiomes/
    └── biome_definition.json
```

---

## 13. Localization

### 13.1 Supported Languages (Launch)
- English (EN)
- Spanish (ES)
- French (FR)
- German (DE)
- Japanese (JP)
- Chinese Simplified (ZH-CN)
- Russian (RU)
- Portuguese (PT-BR)

### 13.2 Translation System
```json
{
    "ui.menu.start": {
        "en": "Start Game",
        "es": "Iniciar Juego",
        "fr": "Commencer le Jeu",
        "de": "Spiel Starten"
    }
}
```

---

## 14. Platform Support

### 14.1 Target Platforms
**Launch**:
- PC (Windows 10/11, Steam)
- PC (Linux, Steam)

**Post-Launch**:
- PlayStation 5
- Xbox Series X/S
- Mac (Apple Silicon)

### 14.2 System Requirements

**Minimum**:
- CPU: Intel i5-7600K / AMD Ryzen 5 1600
- RAM: 16 GB
- GPU: NVIDIA GTX 1060 6GB / AMD RX 580 8GB
- Storage: 50 GB SSD
- DirectX: Version 12

**Recommended**:
- CPU: Intel i7-9700K / AMD Ryzen 7 3700X
- RAM: 32 GB
- GPU: NVIDIA RTX 3070 / AMD RX 6800
- Storage: 100 GB NVMe SSD
- DirectX: Version 12

**Ultra**:
- CPU: Intel i9-12900K / AMD Ryzen 9 5950X
- RAM: 64 GB
- GPU: NVIDIA RTX 4080 / AMD RX 7900 XTX
- Storage: 100 GB NVMe SSD (Gen 4)
- DirectX: Version 12

---

## Conclusion

This technical specification provides a comprehensive blueprint for implementing EDNMS using a custom simulation-first engine architecture. The decision to build a custom engine (detailed in [ENGINE_ARCHITECTURE.md](ENGINE_ARCHITECTURE.md)) enables full control over the simulation core, chunk streaming, and physical persistence systems that are central to the game's vision. The modular architecture allows for parallel development of different systems while maintaining clean interfaces between components.

Key technical priorities:
1. **Robust Physics**: Ensure flight feels precise and responsive
2. **Efficient Streaming**: Seamless transitions between space and planets
3. **Scalable Networking**: Support thousands of concurrent players
4. **Deterministic Generation**: Same seed = same universe for all players
5. **Performance First**: 60 FPS minimum across all platforms

The specifications provided are production-ready and can be directly used as engineering requirements for a development team.
