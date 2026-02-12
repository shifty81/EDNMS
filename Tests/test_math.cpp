#include "test_framework.h"
#include "Engine/Math/Vec3d.h"

TEST(Vec3d, DefaultConstructor) {
    ednms::Vec3d v;
    EXPECT_NEAR(v.x, 0.0, 1e-15);
    EXPECT_NEAR(v.y, 0.0, 1e-15);
    EXPECT_NEAR(v.z, 0.0, 1e-15);
    return true;
}

TEST(Vec3d, Addition) {
    ednms::Vec3d a{1.0, 2.0, 3.0};
    ednms::Vec3d b{4.0, 5.0, 6.0};
    ednms::Vec3d c = a + b;
    EXPECT_NEAR(c.x, 5.0, 1e-9);
    EXPECT_NEAR(c.y, 7.0, 1e-9);
    EXPECT_NEAR(c.z, 9.0, 1e-9);
    return true;
}

TEST(Vec3d, Subtraction) {
    ednms::Vec3d a{5.0, 7.0, 9.0};
    ednms::Vec3d b{1.0, 2.0, 3.0};
    ednms::Vec3d c = a - b;
    EXPECT_NEAR(c.x, 4.0, 1e-9);
    EXPECT_NEAR(c.y, 5.0, 1e-9);
    EXPECT_NEAR(c.z, 6.0, 1e-9);
    return true;
}

TEST(Vec3d, ScalarMultiply) {
    ednms::Vec3d v{1.0, 2.0, 3.0};
    ednms::Vec3d r = v * 2.0;
    EXPECT_NEAR(r.x, 2.0, 1e-9);
    EXPECT_NEAR(r.y, 4.0, 1e-9);
    EXPECT_NEAR(r.z, 6.0, 1e-9);
    return true;
}

TEST(Vec3d, Length) {
    ednms::Vec3d v{3.0, 4.0, 0.0};
    EXPECT_NEAR(v.Length(), 5.0, 1e-9);
    return true;
}

TEST(Vec3d, Normalized) {
    ednms::Vec3d v{0.0, 0.0, 5.0};
    ednms::Vec3d n = v.Normalized();
    EXPECT_NEAR(n.x, 0.0, 1e-9);
    EXPECT_NEAR(n.y, 0.0, 1e-9);
    EXPECT_NEAR(n.z, 1.0, 1e-9);
    return true;
}

TEST(Vec3d, ZeroNormalized) {
    ednms::Vec3d v{0.0, 0.0, 0.0};
    ednms::Vec3d n = v.Normalized();
    EXPECT_NEAR(n.Length(), 0.0, 1e-9);
    return true;
}

TEST(Vec3d, DotProduct) {
    ednms::Vec3d a{1.0, 0.0, 0.0};
    ednms::Vec3d b{0.0, 1.0, 0.0};
    EXPECT_NEAR(a.Dot(b), 0.0, 1e-9);

    ednms::Vec3d c{1.0, 2.0, 3.0};
    ednms::Vec3d d{4.0, 5.0, 6.0};
    EXPECT_NEAR(c.Dot(d), 32.0, 1e-9);
    return true;
}

TEST(Vec3d, CrossProduct) {
    ednms::Vec3d x{1.0, 0.0, 0.0};
    ednms::Vec3d y{0.0, 1.0, 0.0};
    ednms::Vec3d z = x.Cross(y);
    EXPECT_NEAR(z.x, 0.0, 1e-9);
    EXPECT_NEAR(z.y, 0.0, 1e-9);
    EXPECT_NEAR(z.z, 1.0, 1e-9);
    return true;
}

TEST(Vec3d, Distance) {
    ednms::Vec3d a{0.0, 0.0, 0.0};
    ednms::Vec3d b{3.0, 4.0, 0.0};
    EXPECT_NEAR(ednms::Vec3d::Distance(a, b), 5.0, 1e-9);
    return true;
}

TEST(Quatd, Identity) {
    ednms::Quatd q = ednms::Quatd::Identity();
    EXPECT_NEAR(q.w, 1.0, 1e-15);
    EXPECT_NEAR(q.x, 0.0, 1e-15);
    EXPECT_NEAR(q.y, 0.0, 1e-15);
    EXPECT_NEAR(q.z, 0.0, 1e-15);
    return true;
}

TEST(Quatd, IdentityRotation) {
    ednms::Quatd q = ednms::Quatd::Identity();
    ednms::Vec3d v{1.0, 2.0, 3.0};
    ednms::Vec3d r = q.Rotate(v);
    EXPECT_NEAR(r.x, 1.0, 1e-9);
    EXPECT_NEAR(r.y, 2.0, 1e-9);
    EXPECT_NEAR(r.z, 3.0, 1e-9);
    return true;
}

TEST(Quatd, Normalize) {
    ednms::Quatd q{2.0, 0.0, 0.0, 0.0};
    ednms::Quatd n = q.Normalized();
    EXPECT_NEAR(n.Length(), 1.0, 1e-9);
    return true;
}
