#include "Engine/Core/Log.h"
#include "Engine/ECS/ecs_registry.h"
#include "Engine/ECS/components.h"
#include "Engine/Math/Vec3d.h"

int main() {
    ednms::Log::Info("EDNMS Engine starting...");

    // Create the ECS registry
    ednms::ECSRegistry registry;

    // Create a ship entity with Transform and Physics components
    ednms::EntityID ship = registry.CreateEntity();
    registry.AddComponent(ship, ednms::TransformComponent{
        ednms::Vec3d{0.0, 0.0, 0.0},
        ednms::Quatd::Identity()
    });
    registry.AddComponent(ship, ednms::PhysicsComponent{
        ednms::Vec3d{0.0, 0.0, 0.0},  // velocity
        ednms::Vec3d{0.0, 0.0, 0.0},  // angular velocity
        1000.0,                         // mass (kg)
        false                           // not static
    });

    ednms::Log::Info("Ship entity created (ID: " + std::to_string(ship) + ")");

    auto* transform = registry.GetComponent<ednms::TransformComponent>(ship);
    auto* physics = registry.GetComponent<ednms::PhysicsComponent>(ship);

    if (transform && physics) {
        ednms::Log::Info("Ship position: ("
            + std::to_string(transform->position.x) + ", "
            + std::to_string(transform->position.y) + ", "
            + std::to_string(transform->position.z) + ")");
        ednms::Log::Info("Ship mass: " + std::to_string(physics->mass) + " kg");
    }

    ednms::Log::Info("EDNMS Engine initialized. Entities: "
        + std::to_string(registry.EntityCount()));
    ednms::Log::Info("Ready for development. See QUICKSTART.md for next steps.");

    return 0;
}
