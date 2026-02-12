#pragma once
#include "Engine/ECS/ecs_types.h"
#include "Engine/Math/Vec3d.h"
#include <vector>

namespace ednms {

// Core components as specified in ENGINE_ARCHITECTURE.md
// Components are plain-old-data (POD) structs.
// No pointers to other components (use EntityID).

struct TransformComponent {
    Vec3d position;
    Quatd rotation;
};

struct PhysicsComponent {
    Vec3d velocity;
    Vec3d angularVelocity;
    double mass = 1.0;
    bool isStatic = false;
};

struct SurvivalComponent {
    float oxygen = 100.0f;
    float temperature = 37.0f;
    float radiation = 0.0f;
    float health = 100.0f;
};

struct PowerComponent {
    float generated = 0.0f;   // watts
    float consumed = 0.0f;    // watts
    bool powered = false;
};

struct InventorySlot {
    ResourceID type = 0;
    uint32_t quantity = 0;
};

struct InventoryComponent {
    std::vector<InventorySlot> slots;
};

struct OwnershipComponent {
    FactionID owner = 0;
    uint8_t accessMask = 0;
};

struct ConstructionComponent {
    BlueprintID blueprint = 0;
    float progress = 0.0f;   // 0-1
    bool complete = false;
};

struct DockingComponent {
    EntityID dockedTo = INVALID_ENTITY;
    bool locked = false;
};

} // namespace ednms
