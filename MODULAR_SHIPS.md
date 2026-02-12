# EDNMS Modular Ship System
## Procedural Ship Generation, Assembly & Combat Architecture

---

## 1. Core Concept — Ship = Graph of Modules

Every ship is a directed graph of modules, not a single mesh. Ships are procedurally generated from DNA seeds and assembled following faction style rules.

### Module Definition
```cpp
enum class ModuleType {
    Core, Engine, Weapon, Hull, Cargo, Shield, Utility
};

enum class ModuleRole {
    Core, Spine, Wing, Engine, Weapon, Cargo, Cockpit, Fin, Ornament
};

enum class StyleTag {
    Industrial, Sleek, Alien, Chunky, Retro
};

struct Hardpoint {
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;
    bool allowMirror;
};

struct ModuleDef {
    ModuleRole role;
    std::vector<StyleTag> styleTags;
    glm::vec3 bounds;
    std::vector<Hardpoint> hardpoints;
    float mass;
    float thrust;
    float cargo;
    float firepower;
    float powerDraw;
    float powerOutput;
};
```

### Ship Instance
```cpp
struct ModuleInstance {
    ModuleDef* def;
    glm::mat4 transform;
    int parent = -1;
    std::vector<int> children;
};

struct Ship {
    std::vector<ModuleInstance> modules;
};
```

Ships can be traversed for power, damage, AI targeting. Detach modules, destroy sections, cascade failures.

---

## 2. Ship Classes & Silhouette Rules

Ship classes define mandatory structure and visual identity — NMS-style rule-driven kitbashing.

### Ship Class Definitions
```cpp
enum class ShipClass {
    Fighter, Explorer, Hauler, Shuttle, Exotic
};

struct SilhouetteRule {
    glm::vec3 minExtent;
    glm::vec3 maxExtent;
    bool symmetricX;
};

struct ShipClassDef {
    ShipClass type;
    SilhouetteRule silhouette;
    int minWings, minEngines, minWeapons;
    float aggressiveness, cargoBias;
};
```

| Class | Silhouette | Key Feature |
|-------|-----------|-------------|
| Fighter | Wings, visible engines | Fast, symmetric |
| Explorer | Long spine, minimal wings | Range-optimized |
| Hauler | Wide body, cargo pods | Capacity-focused |
| Shuttle | Compact, stubby | Utility |
| Exotic | Rare, asymmetric | Unique shapes |

---

## 3. Ship Size Classes (XS → TITAN)

```
XS  → Fighters / Drones
S   → Frigates
M   → Destroyers
L   → Cruisers
XL  → Battleships
XXL → Dreadnoughts (spinal weapons mandatory)
XXXL → Super-Dreadnoughts (hangars + redundancy)
HUGE → Flagships / Arkships (internal economies, mobile bases)
TITAN → Motherships / World-Eaters (ship made of ships)
```

```cpp
enum class ModuleSize {
    XS, S, M, L, XL, XXL, XXXL, HUGE, TITAN
};
```

Each size tier adds one new structural rule:

| Tier | New Rule Introduced |
|------|-------------------|
| Battleship (XL) | Multi-core |
| Dreadnought (XXL) | Spinal weapons mandatory |
| Super-Dread (XXXL) | Hangars & redundancy |
| Arkship (HUGE) | Internal economies, sub-ship docking |
| Titan (TITAN) | Multi-ship composition, sector-level entity |

### TITAN Ships
A TITAN is not one ship — it's a ship made of ships. Sections simulate independently. Internal docking bays hold fleets. Segment destruction doesn't destroy the whole entity. Think: Homeworld Mothership + Halo Forerunner + EVE Keepstar.

---

## 4. AI Ship Designer

The AI designer follows a strict design script, not random assembly.

### Design Phases (in order)
1. Pick ShipClass
2. Pick Style Theme (faction-locked)
3. Place Core + Spine (skeleton)
4. Add Silhouette Modules (wings, visual identity)
5. Add Functional Modules (weapons, engines to meet stats)
6. Add Ornamentation (fins, antennae — cosmetic only)
7. Validate & Adjust

### AIShipDesigner Class
```cpp
class AIShipDesigner {
public:
    Ship generateShip(const FactionStyle& faction, uint32_t seed);
private:
    ShipDNA generateDNA(const FactionStyle& faction, uint32_t seed);
    void buildSkeleton(Ship& ship, const ShipDNA& dna);
    void addSilhouetteModules(Ship& ship, const ShipDNA& dna);
    void addFunctionalModules(Ship& ship, const ShipDNA& dna);
    void addOrnaments(Ship& ship, const ShipDNA& dna);
    void tuneForCombatRole(Ship& ship, const ShipDNA& dna);
    bool validateShip(const Ship& ship, const ShipDNA& dna);
};
```

### Validation Rules
- Engines must face backward
- Weapons must have firing arcs
- Power must balance (totalDraw ≤ totalOutput)
- Mass must not exceed thrust (thrust ≥ mass × MIN_ACCEL)
- Ship must fit within silhouette bounds
- Center of mass must be near spine

---

## 5. ShipDNA — Seed-Based Identity

```cpp
struct ShipDNA {
    uint32_t seed;
    ShipClass shipClass;
    StyleTag primaryStyle;
    CombatRole combatRole;
    int desiredWeapons, desiredEngines, desiredCargo;
    bool symmetric;
};

enum class CombatRole {
    Brawler,    // short range, tanky
    Kiter,      // fast, long range
    Sniper,     // fragile, high alpha
    Carrier     // utility & drones
};
```

Same seed + same faction = same ship. Always. This enables:
- Save/load in microseconds (serialize DNA, not meshes)
- Multiplayer sync (send seed, regenerate on client)
- Deterministic world state

---

## 6. Faction Style Matrix

Factions lock visual style to prevent "style soup".

```cpp
struct FactionStyle {
    std::vector<StyleTag> allowedStyles;
    float symmetryBias;
    float ornamentChance;
    std::unordered_map<ModuleRole, float> roleBias;
    std::unordered_map<CombatRole, float> combatBias;
};
```

| Faction Style | Allowed Tags | Symmetry | Combat Bias |
|--------------|-------------|----------|-------------|
| Industrial | Industrial, Chunky | 90% | Brawler 60% |
| Sleek Military | Sleek, Retro | 100% | Kiter 70%, Sniper 30% |
| Alien | Alien | 50% | Mixed |

---

## 7. Hardpoint Snapping Math

```cpp
glm::mat4 computeSnapTransform(
    const Hardpoint& parent,
    const Hardpoint& child,
    bool mirror
) {
    glm::vec3 targetForward = -parent.forward;
    glm::vec3 targetUp = parent.up;
    glm::vec3 childForward = child.forward;
    glm::vec3 childUp = child.up;

    if (mirror) {
        childForward.x *= -1.0f;
        childUp.x *= -1.0f;
    }

    glm::quat rot1 = glm::rotation(childForward, targetForward);
    glm::vec3 rotatedUp = rot1 * childUp;
    glm::quat rot2 = glm::rotation(rotatedUp, targetUp);
    glm::quat finalRot = rot2 * rot1;

    glm::vec3 worldPos = parent.position - (finalRot * child.position);

    return glm::translate(glm::mat4(1), worldPos) *
           glm::mat4_cast(finalRot);
}
```

- No gimbal lock, deterministic, mirror-safe
- Reject attachments if forward vectors > 30° off
- Mirror at hardpoint level only, never scale meshes

---

## 8. Module Damage & Cascading Failures

Ships are trees. Damage is structural.

```cpp
struct ModuleHealth {
    float hp, maxHp, armor;
    bool destroyed;
};
```

### Damage Propagation
- Destroy module → depower dependents → detach subtree
- Core destroyed → ship dead
- Reactor destroyed → weapons + shields offline
- Spine destroyed → front section detaches
- Engine destroyed → thrust imbalance

### Cascading Failure Examples

| Hit | Result |
|-----|--------|
| Reactor destroyed | Weapons + shields offline |
| Spine destroyed | Front section detaches as debris |
| Engine destroyed | Thrust imbalance, spin |
| Power conduit destroyed | Local blackout |
| Habitat destroyed | Morale collapse, crew loss |
| Factory destroyed | Ammo starvation |

---

## 9. Binary Ship Serializer

We serialize intent + structure, not meshes or transforms.

### File Format
```
[ShipFileHeader]     — magic 'SHIP' (0x50494853), version, flags, size
[ShipDNABinary]      — 12 bytes: seed, class, style, role, desires, symmetry
[uint16 overrideCount]
[ModuleOverrideBinary × N]  — player/damage diffs (removed, added, replaced)
[uint16 damageCount]
[ModuleDamageBinary × M]    — non-full-health modules only
```

A typical ship save is ~100–300 bytes total. Ships regenerate from DNA on load.

### Networking Bonus
Server sends ShipDNA seed → clients generate same ship → send override diffs only. Deterministic, minimal bandwidth.

---

## 10. Combat Role Tuning

```cpp
void AIShipDesigner::tuneForCombatRole(Ship& ship, const ShipDNA& dna) {
    switch (dna.combatRole) {
    case CombatRole::Brawler:
        ship.addArmorBias();
        ship.addShortRangeWeapons();
        break;
    case CombatRole::Kiter:
        ship.addExtraEngines();
        ship.addLongRangeWeapons();
        break;
    case CombatRole::Sniper:
        ship.reduceMass();
        ship.addHighAlphaWeapons();
        break;
    case CombatRole::Carrier:
        ship.addUtilityBays();
        break;
    }
}
```

---

## 11. Composite Ship AI (TITAN Brains)

A TITAN is a federation of AIs, not one AI.

```
TitanBrain (Strategic)
 ├── SectorBrain (Left Spine)
 │     ├── WeaponController
 │     ├── EngineController
 │     └── DefenseController
 ├── SectorBrain (Right Spine)
 ├── InternalOpsBrain
 │     ├── FactoryAI
 │     ├── HabitatAI
 │     └── HangarAI
 └── ThreatAssessmentBrain
```

If TitanBrain dies → sector brains keep fighting without coordination. Each brain has local sensors, can be destroyed independently, operates with delayed/partial info.

| Brain | Tick Rate |
|-------|-----------|
| TitanBrain | 0.5–1s |
| SectorBrain | 100–200ms |
| Weapon Controllers | 30–60Hz |
| Interior AI | 1–2s |

---

## 12. Interior Module System

Interior modules are economic systems, not decor.

| Module | Produces | Consumes |
|--------|----------|----------|
| Habitat | Crew | Power, food |
| Factory | Ammo, drones | Ore, power |
| Hangar | Fighters | Crew, parts |
| Reactor | Power | Fuel |
| Command | Buffs | Crew |

Destroy habitats → morale collapse. Destroy factories → ammo starvation.

### Boarding Gameplay
Breach specific modules → capture factories (ship loses ammo regen) → sabotage reactors (cascading failures). No "boarding HP bar" — it's systemic.

---

## 13. Player Ship Editor

Same rules as AI designer. Player is a manual AI designer.

### Editor Flow
```
Browse Modules → Hover Hardpoint → Preview Snap → Validate → Commit
```

- Ghost preview: green = valid, red = invalid
- Symmetry toggle (attach mirrored pairs)
- Real-time stats overlay: power balance, thrust/mass, weapon arcs
- Undo/redo via command stack pattern

### Player-Built Ship Saves
```cpp
struct PlayerShipSave {
    ShipDNA dna;
    std::vector<ModuleOverride> overrides;  // manual deviations from AI
};
```

---

## 14. Fleet Doctrine System

Doctrines are data-driven policies that AI interprets. Player fleets and AI factions use the same system.

```cpp
struct FleetDoctrine {
    float preferredRange;
    float cohesion;
    float flankBias;
    float pursuitAggression;
    float retreatThreshold;
    float targetFocus;
    bool protectCapitals;
    bool screenCarriers;
    std::vector<TargetPriority> priorities;
};
```

| Doctrine | Cohesion | Focus | Style |
|----------|----------|-------|-------|
| Line Battle | Tight | Focus fire | Protect capitals |
| Wolfpack | Loose | Kill stragglers | Fast retreat |
| Titan Escort | Absolute | Screen missiles | Sacrifice frigates |

---

## 15. Rendering (GPU Instancing)

All module geometry is procedurally generated and rendered via GPU instancing.

```cpp
struct InstanceData {
    glm::mat4 model;
    glm::vec4 colorMask;
};
// One draw call per module type per LOD level
glDrawElementsInstanced(...);
```

| Distance | LOD Rule |
|----------|---------|
| < 100m | Full procedural module meshes |
| 100–500m | Simplified module meshes |
| > 500m | Merged hull proxy (single baked mesh) |

Destroyed modules: stop instancing, swap to debris mesh, no skinning needed.

---

## 16. OBJ Conventions (Reference Geometry)

The OBJ files in the repository serve as reference shapes during development. All rules below apply to both reference OBJs and procedurally generated module geometry.

| Rule | Value |
|------|-------|
| Forward | +Z |
| Up | +Y |
| Right | +X |
| Origin | Center of mass |
| Units | 1.0 = 1 meter |
| Symmetry plane | X = 0 |

### Hardpoint Naming Convention
Hardpoints are `hp_` prefixed sub-objects:
- `hp_engine_N` — Engine mount points
- `hp_weapon_N` — Weapon hardpoints
- `hp_wing_L` / `hp_wing_R` — Wing attachments
- `hp_spine` — Spine connector for capital modules
- `hp_root` — Where module attaches to parent

### Reference Packs

| Pack | Modules | Scale |
|------|---------|-------|
| `ModularShipModulePack.zip` | core_s, engine_s, weapon_s, wing_s | Small (XS–S) |
| `CruiserBattleshipModulePack.zip` | core_m, spine_m, engine_block_m, turret_m, hangar_m | Medium–Large (M–L) |

---

## 17. Procedural Module Geometry Generation

Since all content is procedurally generated, module meshes are created at runtime from parameterized definitions:

1. **Hull shapes**: Parameterized box hulls with chamfered edges, scaled by module size class
2. **Engine bells**: Conical/cylindrical geometry with exhaust nozzle parameters
3. **Weapon mounts**: Turret base + barrel geometry from weapon type definitions
4. **Wings/Fins**: Extruded profile curves with sweep and taper parameters
5. **Spine segments**: Elongated structural beams with connector geometry

All procedural geometry follows the same hardpoint, orientation, and scale conventions as the reference OBJs.

---

*This document is based on the modular ship design conversation (`modular ship.txt`). See [ENGINE_ARCHITECTURE.md](ENGINE_ARCHITECTURE.md) for the core engine architecture and [TECHNICAL_SPECS.md](TECHNICAL_SPECS.md) for full technical specifications.*
