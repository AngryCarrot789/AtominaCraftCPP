#pragma once
#include "Vector3.h"
class Vector4 {
public:
    Vector4() {}
    explicit Vector4(float b) : x(b), y(b), z(b), w(b) {}
    explicit Vector4(const Vector3& xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
    explicit Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    inline Vector3 XYZ() const { return Vector3(x, y, z); }
    inline Vector3 XYZNormalized() const { return Vector3(x, y, z).Normalized(); }
    inline Vector3 Homogenized() const { return Vector3(x / w, y / w, z / w); }

    inline Vector4 operator*(float b) const {
        return Vector4(x * b, y * b, z * b, w * b);
    }
    inline Vector4 operator/(float b) const {
        return Vector4(x / b, y / b, z / b, w / b);
    }
    inline void operator*=(float b) {
        x *= b; y *= b; z *= b; w *= b;
    }
    inline void operator/=(float b) {
        x /= b; y /= b; z /= b; w /= b;
    }

    inline float Dot(const Vector4& b) const { return x * b.x + y * b.y + z * b.z + w * b.w; }

    //Components
    float x, y, z, w;
};