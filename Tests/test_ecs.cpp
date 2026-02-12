#include "test_framework.h"
#include "Engine/ECS/ecs_registry.h"
#include "Engine/ECS/components.h"

TEST(ECS, CreateEntity) {
    ednms::ECSRegistry registry;
    ednms::EntityID e = registry.CreateEntity();
    EXPECT_NE(e, ednms::INVALID_ENTITY);
    EXPECT_EQ(registry.EntityCount(), 1u);
    return true;
}

TEST(ECS, DestroyEntity) {
    ednms::ECSRegistry registry;
    ednms::EntityID e = registry.CreateEntity();
    registry.DestroyEntity(e);
    EXPECT_EQ(registry.EntityCount(), 0u);
    EXPECT_FALSE(registry.HasEntity(e));
    return true;
}

TEST(ECS, AddAndGetComponent) {
    ednms::ECSRegistry registry;
    ednms::EntityID e = registry.CreateEntity();

    ednms::TransformComponent t;
    t.position = {1.0, 2.0, 3.0};
    registry.AddComponent(e, t);

    auto* got = registry.GetComponent<ednms::TransformComponent>(e);
    EXPECT_TRUE(got != nullptr);
    EXPECT_NEAR(got->position.x, 1.0, 1e-9);
    EXPECT_NEAR(got->position.y, 2.0, 1e-9);
    EXPECT_NEAR(got->position.z, 3.0, 1e-9);
    return true;
}

TEST(ECS, RemoveComponent) {
    ednms::ECSRegistry registry;
    ednms::EntityID e = registry.CreateEntity();
    registry.AddComponent(e, ednms::PhysicsComponent{});

    EXPECT_TRUE(registry.HasComponent<ednms::PhysicsComponent>(e));
    registry.RemoveComponent<ednms::PhysicsComponent>(e);
    EXPECT_FALSE(registry.HasComponent<ednms::PhysicsComponent>(e));
    return true;
}

TEST(ECS, MultipleEntitiesIndependent) {
    ednms::ECSRegistry registry;
    ednms::EntityID e1 = registry.CreateEntity();
    ednms::EntityID e2 = registry.CreateEntity();

    registry.AddComponent(e1, ednms::TransformComponent{{10, 0, 0}, {}});
    registry.AddComponent(e2, ednms::TransformComponent{{20, 0, 0}, {}});

    auto* t1 = registry.GetComponent<ednms::TransformComponent>(e1);
    auto* t2 = registry.GetComponent<ednms::TransformComponent>(e2);
    EXPECT_NEAR(t1->position.x, 10.0, 1e-9);
    EXPECT_NEAR(t2->position.x, 20.0, 1e-9);
    return true;
}

TEST(ECS, ComponentMaskTracking) {
    ednms::ECSRegistry registry;
    ednms::EntityID e = registry.CreateEntity();

    registry.AddComponent(e, ednms::TransformComponent{});
    registry.AddComponent(e, ednms::PhysicsComponent{});

    const auto& mask = registry.GetMask(e);
    EXPECT_TRUE(mask.count() == 2);
    return true;
}

TEST(ECS, DestroyRemovesComponents) {
    ednms::ECSRegistry registry;
    ednms::EntityID e = registry.CreateEntity();
    registry.AddComponent(e, ednms::SurvivalComponent{});
    registry.DestroyEntity(e);

    EXPECT_EQ(registry.EntityCount(), 0u);
    return true;
}
