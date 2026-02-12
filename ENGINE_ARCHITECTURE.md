# EDNMS Engine Architecture
## Custom Simulation-First Engine Design

---

## Core Philosophy

These five rules govern every architectural decision. They are non-negotiable.

1. **Simulation first, visuals second**
2. **Everything is data-driven**
3. **No system depends on rendering**
4. **If it can't be saved/loaded, it doesn't exist**
5. **One solar system must run headless**

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

/Tests/
```

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

- ImGui (even headless with dummy renderer)
- Toggleable overlays
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
