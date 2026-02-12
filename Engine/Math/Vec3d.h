#pragma once
#include <cmath>
#include <cstddef>

namespace ednms {

// Double-precision 3D vector for space-scale coordinates
struct Vec3d {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Vec3d() = default;
    Vec3d(double x, double y, double z) : x(x), y(y), z(z) {}

    Vec3d operator+(const Vec3d& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vec3d operator-(const Vec3d& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vec3d operator*(double scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }

    Vec3d& operator+=(const Vec3d& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    Vec3d& operator-=(const Vec3d& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    Vec3d& operator*=(double scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    bool operator==(const Vec3d& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vec3d& other) const {
        return !(*this == other);
    }

    double Dot(const Vec3d& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3d Cross(const Vec3d& other) const {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }

    double LengthSquared() const {
        return x * x + y * y + z * z;
    }

    double Length() const {
        return std::sqrt(LengthSquared());
    }

    Vec3d Normalized() const {
        double len = Length();
        if (len < 1e-15) return {0.0, 0.0, 0.0};
        return *this * (1.0 / len);
    }

    static double Distance(const Vec3d& a, const Vec3d& b) {
        return (a - b).Length();
    }
};

// Double-precision quaternion for rotation
struct Quatd {
    double w = 1.0;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Quatd() = default;
    Quatd(double w, double x, double y, double z) : w(w), x(x), y(y), z(z) {}

    static Quatd Identity() {
        return {1.0, 0.0, 0.0, 0.0};
    }

    Quatd operator*(const Quatd& q) const {
        return {
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        };
    }

    double LengthSquared() const {
        return w * w + x * x + y * y + z * z;
    }

    double Length() const {
        return std::sqrt(LengthSquared());
    }

    Quatd Normalized() const {
        double len = Length();
        if (len < 1e-15) return Identity();
        double inv = 1.0 / len;
        return {w * inv, x * inv, y * inv, z * inv};
    }

    Quatd Conjugate() const {
        return {w, -x, -y, -z};
    }

    // Rotate a vector by this quaternion
    Vec3d Rotate(const Vec3d& v) const {
        Quatd qv{0.0, v.x, v.y, v.z};
        Quatd result = *this * qv * Conjugate();
        return {result.x, result.y, result.z};
    }
};

} // namespace ednms
