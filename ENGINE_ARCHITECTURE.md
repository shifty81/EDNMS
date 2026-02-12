# EDNMS Engine Architecture
## Custom Simulation-First Engine Design

---

## Core Philosophy

These rules govern every architectural decision. They are non-negotiable.

1. **Simulation first, visuals second**
2. **Everything is data-driven**
3. **No system depends on rendering**
4. **If it can't be saved/loaded, it doesn't exist**
5. **One solar system must run headless**
6. **Everything is procedurally generated or AI-created — no pre-made assets**

All game content — geometry, textures, ships, creatures, planets, stations, flora, fauna, terrain, interiors, and UI elements — is generated at runtime through procedural algorithms and AI systems. There are no handcrafted 3D models, no imported artist assets, and no external content libraries. The OBJ module packs in the repository serve as reference geometry and test placeholders that will be replaced by the procedural generation pipeline.

This is not a general-purpose engine, not a Unity/Unreal competitor, and not a tech demo. It is a **simulation-first engine** designed specifically for a space survival / colonization game. Everything exists to serve physical persistence, scale, logistics, ownership, and long-term simulation.

---

## Project Layout

```
/Engine/
  /Core/          - App lifecycle, time step, job system, logging, assertions
  /ECS/           - Entity Component System
  /Math/          - Double precision vectors, deterministic math, floating origin
  /IO/            - Binary serialization, chunked world saves
  /Platform/      - Platform abstraction

/Simulation/
  /World/         - World hierarchy, chunks
  /Survival/      - O2, temperature, radiation, health
  /Power/         - Power network graphs
  /Logistics/     - Physical resource transport
  /Construction/  - Staged building
  /Ownership/     - Faction/system ownership

/Game/
  /Player/        - Input, camera, UI glue
  /Ships/         - Ship entities
  /Bases/         - Base entities
  /Stations/      - Station entities

/Renderer/        - Initially minimal (OpenGL/Vulkan, debug meshes)

/Tools/
  /WorldEditor/
  /AssetPipeline/

/Data/
  /Definitions/
  /Blueprints/
  /ShipModules/     - OBJ ship module packs (see Ship Assets below)

/Tests/
```

### Ship Assets

Two modular ship OBJ packs are included in the repository root:

- **`CruiserBattleshipModulePack.zip`** — Large capital ship modules: `core_m.obj`, `spine_m.obj`, `engine_block_m.obj`, `turret_m.obj`, `hangar_m.obj`
- **`ModularShipModulePack.zip`** — Small ship modules: `core_s.obj`, `engine_s.obj`, `weapon_s.obj`, `wing_s.obj`

These use a hardpoint naming convention (`hp_` prefix) for attachment points, enabling modular ship assembly through the construction system.

### Dependency Rules

- **Simulation does NOT depend on Engine.**
- **Game does NOT depend on Renderer.**

These constraints are deliberate. The simulation layer is the soul of the game and must remain engine-agnostic. The renderer is swappable.

---

## ECS Architecture

### Philosophy

- Data-oriented
- No inheritance trees
- No "God entities"
- Systems own behavior, components own state
- Simulation-safe headless

### Core ECS Types

```cpp
// ecs_types.h
#pragma once
#include <cstdint>

using EntityID = uint64_t;
static constexpr EntityID INVALID_ENTITY = 0;
```

```cpp
// ecs_component_mask.h
#pragma once
#include <bitset>

static constexpr size_t MAX_COMPONENTS = 64;
using ComponentMask = std::bitset<MAX_COMPONENTS>;
```

### Component Rules

- Components are plain-old-data (POD) structs
- No pointers to other components (use `EntityID`)
- Serializable by default

### Core Components

```cpp
struct TransformComponent {
    Vec3d position;     // double precision
    Quatd rotation;
};
// No scale. Buildings don't scale. Ships don't scale. Simplicity wins.

struct PhysicsComponent {
    Vec3d velocity;
    Vec3d angularVelocity;
    double mass;
    bool isStatic;
};

struct SurvivalComponent {
    float oxygen;
    float temperature;
    float radiation;
    float health;
};

struct PowerComponent {
    float generated;   // watts
    float consumed;    // watts
    bool powered;
};

struct InventorySlot {
    ResourceID type;
    uint32_t quantity;
};

struct InventoryComponent {
    std::vector<InventorySlot> slots;
};

struct OwnershipComponent {
    FactionID owner;
    uint8_t accessMask;
};

struct ConstructionComponent {
    BlueprintID blueprint;
    float progress;      // 0–1
    bool complete;
};

struct DockingComponent {
    EntityID dockedTo;
    bool locked;
};
```

### ECS Registry (Header-Only, C++17)

```cpp
// ecs_registry.h
#pragma once
#include <unordered_map>
#include <vector>
#include <cassert>
#include "ecs_types.h"
#include "ecs_component_mask.h"

class ECSRegistry {
public:
    EntityID CreateEntity() {
        EntityID id = ++m_nextEntity;
        m_entityMasks[id] = {};
        return id;
    }

    void DestroyEntity(EntityID id) {
        m_entityMasks.erase(id);
        for (auto& pool : m_componentPools) {
            pool.second.erase(id);
        }
    }

    template<typename T>
    void RegisterComponent() {
        const size_t typeId = GetComponentTypeID<T>();
        m_componentPools[typeId] = {};
    }

    template<typename T>
    void AddComponent(EntityID id, const T& component) {
        const size_t typeId = GetComponentTypeID<T>();
        m_componentPools[typeId][id] = component;
        m_entityMasks[id].set(typeId);
    }

    template<typename T>
    T* GetComponent(EntityID id) {
        const size_t typeId = GetComponentTypeID<T>();
        auto& pool = m_componentPools[typeId];
        auto it = pool.find(id);
        return it != pool.end() ? &std::any_cast<T&>(it->second) : nullptr;
    }

    const ComponentMask& GetMask(EntityID id) const {
        return m_entityMasks.at(id);
    }

private:
    EntityID m_nextEntity = 0;

    std::unordered_map<EntityID, ComponentMask> m_entityMasks;
    std::unordered_map<size_t, std::unordered_map<EntityID, std::any>> m_componentPools;

    template<typename T>
    static size_t GetComponentTypeID() {
        static size_t id = s_nextTypeID++;
        return id;
    }

    static inline size_t s_nextTypeID = 0;
};
```

This ECS is intentionally boring — explicit, serializable, and easy to replace later with SoA pools.

### System Update Order

```
InputSystem
    ↓
PhysicsSystem
    ↓
SurvivalSystem
    ↓
PowerSystem
    ↓
LogisticsSystem
    ↓
ConstructionSystem
    ↓
OwnershipSystem
    ↓
SaveSystem
```

**No circular dependencies. Ever.**

---

## Chunk Streaming Model

This is the backbone of scalability.

### World Hierarchy

```
Galaxy
 └── System
     ├── Star
     ├── Planet(s)
     │    └── Regions
     │         └── Chunks
     └── Orbital Space
          └── Space Chunks
```

### Planet Chunks

- Fixed-size 3D grid
- Suggested: 64m³ or 128m³
- Terrain + entities decoupled
- Static terrain (v0)

### Space Chunks

- Much larger (km-scale)
- No terrain
- Used for: stations, ships, fleets, orbital structures

### Chunk States

```cpp
enum class ChunkState {
    Unloaded,
    LowFidelitySim,
    FullSim
};
```

### Streaming Priority

Chunks are scored every frame (or second):

```
priority =
    + PlayerDistanceWeight
    + OwnershipWeight
    + ActiveLogisticsWeight
    + ConstructionWeight
```

Ownership keeps chunks alive even if the player leaves.

### Chunk Scheduler Skeleton

```cpp
class ChunkScheduler {
public:
    void Update(double now) {
        for (auto& chunk : m_chunks) {
            double score = EvaluatePriority(chunk);

            if (score > FULL_THRESHOLD)
                Promote(chunk, ChunkSimState::Full);
            else if (score > LOW_THRESHOLD)
                Promote(chunk, ChunkSimState::LowFidelity);
            else
                Demote(chunk);
        }
    }

private:
    double EvaluatePriority(const ChunkRuntime& chunk);
    void Promote(ChunkRuntime& chunk, ChunkSimState target);
    void Demote(ChunkRuntime& chunk);
};
```

---

## Save/Load Binary Format

### Design Goals

- Chunk-local
- Forward compatible
- Streaming friendly
- No reflection

### File Layout

```
/Saves/World01/
 ├── world.meta
 ├── system_0001/
 │    ├── planet_earth/
 │    │    ├── chunk_0001.bin
 │    │    ├── chunk_0002.bin
 │    ├── orbit.bin
```

### Chunk File Structure

```
[ChunkHeader]
[EntityTable]
[ComponentData...]
```

### ChunkHeader

```cpp
// chunk_format.h
#pragma once
#include <cstdint>

static constexpr uint32_t CHUNK_MAGIC = 0x43484B31; // "CHK1"

struct ChunkHeader {
    uint32_t magic;          // sanity check
    uint32_t version;        // format version
    uint64_t chunkID;
    uint32_t entityCount;
    uint32_t componentCount;
};
```

### EntityRecord

```cpp
struct EntityRecord {
    EntityID id;
    ComponentMask mask;
};
// Written contiguously: [EntityRecord x N]
```

### Component Serialization Contract

Every component must implement:

```cpp
struct ComponentX {
    static constexpr uint32_t VERSION = 1;

    void Serialize(BinaryWriter& w) const;
    void Deserialize(BinaryReader& r, uint32_t version);
};
```

No reflection magic. Explicit is safer.

### SaveChunk Skeleton

```cpp
void SaveChunk(const Chunk& chunk, BinaryWriter& w) {
    ChunkHeader header{};
    header.magic = CHUNK_MAGIC;
    header.version = 1;
    header.chunkID = chunk.id;
    header.entityCount = chunk.entities.size();

    w.Write(header);

    // Entity table
    for (EntityID e : chunk.entities) {
        EntityRecord rec{ e, registry.GetMask(e) };
        w.Write(rec);
    }

    // Component data
    for (EntityID e : chunk.entities) {
        const auto& mask = registry.GetMask(e);
        for (size_t i = 0; i < MAX_COMPONENTS; ++i) {
            if (mask.test(i)) {
                registry.SerializeComponentByIndex(e, i, w);
            }
        }
    }
}
```

### LoadChunk Skeleton

```cpp
void LoadChunk(Chunk& chunk, BinaryReader& r) {
    ChunkHeader header;
    r.Read(header);

    assert(header.magic == CHUNK_MAGIC);

    std::vector<EntityRecord> entities(header.entityCount);
    r.ReadArray(entities);

    for (auto& rec : entities) {
        registry.CreateEntityWithID(rec.id);
        registry.SetMask(rec.id, rec.mask);
    }

    for (auto& rec : entities) {
        for (size_t i = 0; i < MAX_COMPONENTS; ++i) {
            if (rec.mask.test(i)) {
                registry.DeserializeComponentByIndex(
                    rec.id, i, r, header.version
                );
            }
        }
    }
}
```

### Save Rules

- Only save dirty chunks
- Flush on chunk unload
- Background IO thread
- **Never block simulation**

---

## Power Network Graph

Power networks are graphs, not abstract "base has power" flags.

### Power Node Types

```cpp
enum class PowerNodeType {
    Generator,
    Consumer,
    Storage,
    Relay
};

struct PowerNode {
    EntityID entity;
    PowerNodeType type;
    float capacity;
    float current;
};
```

### Power Edge (Cable / Conduit)

```cpp
struct PowerEdge {
    EntityID from;
    EntityID to;
    float maxThroughput;
};
```

### Power Graph

```cpp
struct PowerGraph {
    std::vector<PowerNode> nodes;
    std::vector<PowerEdge> edges;
};
```

### Simulation Step

1. Reset all node `current`
2. Sum generators
3. Traverse graph (BFS)
4. Distribute power respecting throughput
5. Brownout consumers if insufficient

### Benefits

- Damage breaks edges
- Power routing matters
- Visual debug overlays
- Scales to stations & planets

---

## Low-Fidelity Simulation

Low-fidelity simulation is not "paused" — it's coarsened.

### Chunk Sim Modes

| Mode     | Tick Rate | Systems                                  |
|----------|-----------|------------------------------------------|
| Full     | Per-frame | All                                      |
| Low      | 5–60s     | Survival, Power, Logistics, Construction |
| Unloaded | None      | Frozen                                   |

### Low-Fidelity Rules

**Disabled:**
- Physics integration
- Collision
- Player input
- AI pathing

**Enabled (Simplified):**
- Power generation/consumption
- Resource transport (batch)
- Construction progress
- Life support drain

### Transition Rules

| Transition  | Action         |
|-------------|----------------|
| Low → Full  | Rehydrate      |
| Full → Low  | Collapse state |

Example: Ships in transit become route timers. Physics velocity zeroed. This avoids discontinuities.

---

## Task/Job System

This is how you use all cores without breaking determinism.

### Job Categories

| Job Type        | Thread          |
|-----------------|-----------------|
| Physics         | Worker          |
| Survival        | Worker          |
| Power           | Worker          |
| Logistics       | Worker          |
| Save IO         | Background      |
| Chunk Streaming | Main → Worker   |

### Job Interface

```cpp
struct Job {
    virtual void Execute() = 0;
};
```

### JobSystem Skeleton

```cpp
class JobSystem {
public:
    void Submit(Job* job);
    void WaitAll();
};
```

### Deterministic Rule

**Jobs may NOT write to the same component type concurrently.**

Use:
- Phase barriers
- Double-buffered component data
- Chunk-level locks

### Frame Pipeline

```
[Input]
    ↓
[Physics Jobs]
    ↓
[Survival + Power Jobs]
    ↓
[Logistics Jobs]
    ↓
[Construction Jobs]
    ↓
[Chunk Save Jobs]
    ↓
[Render / Debug]
```

---

## Procedural Generation & AI Content Pipeline

All game content is generated procedurally or by AI at runtime. There are no pre-made artist assets.

### What Is Procedurally Generated

| Content | Generation Method | Seed-Based |
|---------|------------------|------------|
| **Planets** | Noise functions (Perlin, Simplex, Worley) for terrain, atmosphere, biomes | Yes |
| **Stars & Systems** | Stellar classification rules + orbital mechanics | Yes |
| **Ship Modules** | Parameterized geometry from module definitions (box hulls, engine bells, turret mounts) | Yes |
| **Ship Assembly** | AI Ship Designer using DNA seeds, faction styles, and silhouette rules | Yes |
| **Creatures** | Procedural body plans with limb/joint graphs and behavioral AI | Yes |
| **Flora** | L-system growth algorithms with biome-appropriate parameters | Yes |
| **Terrain** | Chunked heightmap + voxel generation with erosion simulation | Yes |
| **Textures & Materials** | Procedural PBR generation (noise-based color, roughness, normal maps) | Yes |
| **Structures & Ruins** | Grammar-based building generation for derelicts, outposts, stations | Yes |
| **Station Interiors** | Modular room graph generation from station module types | Yes |
| **Weather** | Atmospheric simulation driven by planet parameters | No (dynamic) |
| **NPC Behavior** | AI decision trees driven by faction doctrine and survival rules | No (dynamic) |

### Generation Principles

1. **Seed determinism**: Same seed always produces the same result — critical for save/load, networking, and DNA-based ship serialization
2. **LOD-aware generation**: Generate only what the player can perceive at current distance; detail increases on approach
3. **GPU-accelerated where possible**: Terrain, textures, and noise generation run on compute shaders
4. **Lazy evaluation**: Content generates on demand as chunks load, not upfront
5. **AI-driven variation**: AI systems (ship designer, creature generator, ruin placer) use heuristic rules to ensure content is coherent, not random noise

### Procedural Geometry Pipeline

```
[Seed + Parameters]
    ↓
[Noise / Algorithm]
    ↓
[Vertex Buffer Generation]
    ↓
[Normal Calculation]
    ↓
[Procedural Material Assignment]
    ↓
[LOD Variants (reduce on distance)]
    ↓
[GPU Upload → Render]
```

No `.fbx`, no `.blend`, no imported meshes in the final game. The OBJ files in the repository (`CruiserBattleshipModulePack.zip`, `ModularShipModulePack.zip`) serve as reference shapes and test geometry during development — the procedural pipeline will generate equivalent geometry at runtime.

---

## Custom UI / HUD System

All GUI, HUD, and debug overlays are built from scratch in C++ and OpenGL — no third-party UI libraries (no ImGui, no Dear ImGui, no external widget kits).

### Design Principles

1. **Engine-native**: UI rendering uses the same OpenGL pipeline as the game
2. **Data-driven**: Widget layouts defined in data files, not hardcoded
3. **Layered**: Game HUD, menus, and debug overlays are separate render layers
4. **Resolution-independent**: All UI elements use normalized coordinates and scale to any resolution

### UI Layers (Render Order)

| Layer | Purpose | Examples |
|-------|---------|----------|
| 0 — Game World | 3D scene | Ships, planets, modules |
| 1 — World HUD | In-world overlays | Hardpoint markers, power flow lines, ship labels |
| 2 — Screen HUD | Player status | Speed, oxygen, health, power distribution, compass |
| 3 — Menus | Full-screen UI | Inventory, ship editor, galaxy map, building mode |
| 4 — Debug | Dev overlays | ECS inspector, chunk visualizer, power overlay, FPS counter |

### Core UI Components

- **Text renderer**: Bitmap font atlas with OpenGL quad batching
- **Rect / panel system**: Colored and textured quads with borders
- **Slider / bar widgets**: For health, oxygen, power, construction progress
- **List / grid views**: For inventory, module browser, market
- **Tooltip system**: Context-sensitive hover information
- **Input handling**: Mouse + keyboard event routing per layer

### HUD Elements (In-Game)

```
┌──────────────────────────────────────────────┐
│  [Speed]        [Crosshair]        [Shields]  │
│   250m/s            ⊕               |||||||   │
│                                     |||||||   │
│  [Thrust]                           [Hull]    │
│   ▓▓▓▓▓░░░                         ========  │
│                                               │
│  [Target Info]                [System Info]   │
│   Enemy Ship                    Oxygen: 95%   │
│   Hull: 60%                     Temp: 22°C    │
│   Shield: 40%                   Hazard: None  │
│                                               │
│  [Power: SYS/ENG/WEP]       [Construction]   │
│   ██░░ ░░██ ██░░              Stage 3/5 47%  │
└──────────────────────────────────────────────┘
```

### Ship Editor UI

```
┌──────────────────────────────────────────────┐
│  Ship Editor                                  │
│  ┌──────────────┐  ┌──────────────────────┐  │
│  │  Modules     │  │  [3D Preview]         │  │
│  │  - Core      │  │   Ghost snap preview  │  │
│  │  - Engine    │  │   Green = valid       │  │
│  │  - Weapon    │  │   Red = invalid       │  │
│  │  - Wing      │  │                       │  │
│  └──────────────┘  └──────────────────────┘  │
│  ┌──────────────────────────────────────────┐│
│  │ Power: OK | Thrust/Mass: 1.2 | Sym: ON  ││
│  └──────────────────────────────────────────┘│
│  [Attach] [Mirror] [Remove] [Undo] [Redo]    │
└──────────────────────────────────────────────┘
```

### Building Mode UI

```
┌──────────────────────────────────────────────┐
│  Building Mode                                │
│  ┌──────────────┐  ┌──────────────┐          │
│  │  Structures  │  │  Resources   │          │
│  │  - Floor     │  │  Iron: 500   │          │
│  │  - Wall      │  │  Carbon: 300 │          │
│  │  - Hangar    │  │  Ferrite: 50 │          │
│  └──────────────┘  └──────────────┘          │
│  [Stage: Framework 62%]                       │
│  [Place] [Rotate] [Delete] [Exit]            │
└──────────────────────────────────────────────┘
```

---

## Debug Tooling

If you skip this, you will regret it. Build debug tools early.

### Mandatory Debug Views

**ECS Inspector**
- Select entity
- View/edit components live
- Show owning chunk

**Chunk Visualizer**
- Chunk bounds
- State colors:
  - Red = unloaded
  - Yellow = low sim
  - Green = full sim

**Power Overlay**
- Power graph lines
- Flow direction
- Overload points

**Save/Load Diff Tool**
- Compare chunk binary before/after
- Highlight component deltas

### Tech

- Custom C++/OpenGL UI system (no third-party GUI libraries)
- Toggleable overlays rendered via the engine's own UI layer
- Zero gameplay dependencies

---

## Why Not Unreal/Unity/Godot/Custom-from-Scratch

**Unreal** is good — but the simulation core should be engine-agnostic. Unreal works best as a renderer and toolchain, not as the brain of a simulation-heavy game. Unreal traps include coupling gameplay to UObject, fighting GC with large worlds, and building systems without data-driven design.

**Unity** has a weak large-world story, ECS is half-finished, and the licensing situation is unstable for a multi-year project.

**Godot** is promising but lacks the 3D maturity needed for a game of this scale (no LOD pipeline, no streaming, limited physics).

**Full custom engine from scratch** is viable technically but enormous practically. You'd build all tooling before the game even exists.

### The Hybrid Approach (What We're Doing)

Write the simulation core in pure, engine-agnostic C++. Use a minimal renderer initially. This is how Factorio, RimWorld, and many proprietary studios work:

```
[Simulation Core] ←→ [Renderer / Tools]
```

The simulation core owns: world simulation, economy, survival systems, build logic, ownership, logistics, and save/load. The renderer is just a renderer. If you ever need to swap to Unreal, Vulkan, or anything else — you already own the hardest logic.
