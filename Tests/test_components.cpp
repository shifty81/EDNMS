#include "test_framework.h"
#include "Engine/ECS/ecs_registry.h"
#include "Engine/ECS/components.h"

TEST(Components, TransformDefault) {
    ednms::TransformComponent t;
    EXPECT_NEAR(t.position.x, 0.0, 1e-15);
    EXPECT_NEAR(t.rotation.w, 1.0, 1e-15);
    return true;
}

TEST(Components, PhysicsDefault) {
    ednms::PhysicsComponent p;
    EXPECT_NEAR(p.mass, 1.0, 1e-15);
    EXPECT_FALSE(p.isStatic);
    return true;
}

TEST(Components, SurvivalDefault) {
    ednms::SurvivalComponent s;
    EXPECT_NEAR(s.oxygen, 100.0f, 1e-5);
    EXPECT_NEAR(s.health, 100.0f, 1e-5);
    EXPECT_NEAR(s.temperature, 37.0f, 1e-5);
    EXPECT_NEAR(s.radiation, 0.0f, 1e-5);
    return true;
}

TEST(Components, PowerDefault) {
    ednms::PowerComponent pw;
    EXPECT_NEAR(pw.generated, 0.0f, 1e-5);
    EXPECT_NEAR(pw.consumed, 0.0f, 1e-5);
    EXPECT_FALSE(pw.powered);
    return true;
}

TEST(Components, InventoryAddSlot) {
    ednms::InventoryComponent inv;
    EXPECT_EQ(inv.slots.size(), 0u);
    inv.slots.push_back({1, 50});
    EXPECT_EQ(inv.slots.size(), 1u);
    EXPECT_EQ(inv.slots[0].quantity, 50u);
    return true;
}

TEST(Components, ConstructionProgress) {
    ednms::ConstructionComponent c;
    EXPECT_NEAR(c.progress, 0.0f, 1e-5);
    EXPECT_FALSE(c.complete);
    c.progress = 1.0f;
    c.complete = true;
    EXPECT_NEAR(c.progress, 1.0f, 1e-5);
    EXPECT_TRUE(c.complete);
    return true;
}

TEST(Components, DockingDefault) {
    ednms::DockingComponent d;
    EXPECT_EQ(d.dockedTo, ednms::INVALID_ENTITY);
    EXPECT_FALSE(d.locked);
    return true;
}

TEST(Components, OwnershipDefault) {
    ednms::OwnershipComponent o;
    EXPECT_EQ(o.owner, 0u);
    EXPECT_EQ(o.accessMask, 0u);
    return true;
}
