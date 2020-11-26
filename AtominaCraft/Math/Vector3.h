#pragma once
#include <cmath>
#include <cfloat>
#include <cassert>
#include "../GameHeader.h"
#include "Maths.h"

class Vector3 {
public:
    //Constructors
    Vector3() : y(0), x(0), z(0) {}
    explicit Vector3(float b) : x(b), y(b), z(b) {}
    explicit Vector3(const float* b) : x(b[0]), y(b[1]), z(b[2]) {}
    explicit Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    //General
    inline static Vector3 Zero() { return Vector3(0.0f); }
    inline static Vector3 Ones() { return Vector3(1.0f); }
    inline static Vector3 UnitX() { return Vector3(1, 0, 0); }
    inline static Vector3 UnitY() { return Vector3(0, 1, 0); }
    inline static Vector3 UnitZ() { return Vector3(0, 0, 1); }
    inline static Vector3 Up()       { return Vector3( 0,  1,  0); }
    inline static Vector3 Down()     { return Vector3( 0, -1,  0); }
    inline static Vector3 Left()     { return Vector3( 1,  0,  0); }
    inline static Vector3 Right()    { return Vector3(-1,  0,  0); }
    inline static Vector3 Forward()  { return Vector3( 0,  0,  1); }
    inline static Vector3 Backward() { return Vector3( 0,  0, -1); }

    //Setters
    inline void Set(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
    inline void SetZero() { x = 0.0f; y = 0.0f; z = 0.0f; }
    inline void SetOnes() { x = 1.0f; y = 1.0f; z = 1.0f; }
    inline void SetUnitX() { x = 1.0f; y = 0.0f; z = 0.0f; }
    inline void SetUnitY() { x = 0.0f; y = 1.0f; z = 0.0f; }
    inline void SetUnitZ() { x = 0.0f; y = 0.0f; z = 1.0f; }

    //Basic operatios
    inline Vector3 operator+(float b) const {
        return Vector3(x + b, y + b, z + b);
    }
    inline Vector3 operator-(float b) const {
        return Vector3(x - b, y - b, z - b);
    }
    inline Vector3 operator*(float b) const {
        return Vector3(x * b, y * b, z * b);
    }
    inline Vector3 operator/(float b) const {
        return Vector3(x / b, y / b, z / b);
    }
    inline Vector3 operator+(const Vector3& b) const {
        return Vector3(x + b.x, y + b.y, z + b.z);
    }
    inline Vector3 operator-(const Vector3& b) const {
        return Vector3(x - b.x, y - b.y, z - b.z);
    }
    inline Vector3 operator*(const Vector3& b) const {
        return Vector3(x * b.x, y * b.y, z * b.z);
    }
    inline Vector3 operator/(const Vector3& b) const {
        return Vector3(x / b.x, y / b.y, z / b.z);
    }
    inline void operator+=(float b) {
        x += b; y += b; z += b;
    }
    inline void operator-=(float b) {
        x -= b; y -= b; z -= b;
    }
    inline void operator*=(float b) {
        x *= b; y *= b; z *= b;
    }
    inline void operator/=(float b) {
        x /= b; y /= b; z /= b;
    }
    inline void operator+=(const Vector3& b) {
        x += b.x; y += b.y; z += b.z;
    }
    inline void operator-=(const Vector3& b) {
        x -= b.x; y -= b.y; z -= b.z;
    }
    inline void operator*=(const Vector3& b) {
        x *= b.x; y *= b.y; z *= b.z;
    }
    inline void operator/=(const Vector3& b) {
        x /= b.x; y /= b.y; z /= b.z;
    }
    inline Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

    //Vector algebra
    inline float Dot(const Vector3& b) const {
        return x * b.x + y * b.y + z * b.z;
    }
    inline Vector3 Cross(const Vector3& b) const {
        return Vector3(y * b.z - z * b.y,
                       z * b.x - x * b.z,
                       x * b.y - y * b.x);
    }
    inline float MagSq() const {
        return x * x + y * y + z * z;
    }
    inline float Mag() const {
        return std::sqrt(MagSq());
    }

    //Normalization
    inline void Normalize() {
        (*this) /= Mag();
    }
    inline void NormalizeSafe() {
        (*this) /= (Mag() + FLT_EPSILON);
    }
    inline Vector3 Normalized() const {
        return (*this) / Mag();
    }
    inline Vector3 NormalizedSafe() const {
        return (*this) / (Mag() + FLT_EPSILON);
    }
    inline float Angle(const Vector3& b) const {
        return std::acos(Normalized().Dot(b.Normalized()));
    }
    inline float AngleSafe(const Vector3& b) const {
        return std::acos(NormalizedSafe().Dot(b.NormalizedSafe()));
    }
    inline void ClampMagnitude(float m) {
        assert(m > 0.0f);
        const float r = MagSq() / (m * m);
        if (r > 1.0f) {
            (*this) /= std::sqrt(r);
        }
    }

    inline Vector3 DegreesToRadians() {
        return *this / Maths::DEG_RAD_CONST;
    }
    inline Vector3 RadiansToDegrees() {
        return *this * Maths::DEG_RAD_CONST;
    }

    //Other
    inline bool IsNDC() const {
        return (x > -1.0f && x < 1.0f && y > -1.0f && y < 1.0f && z > -1.0f && z < 1.0f);
    }

    //Components
    float x, y, z;
};

inline Vector3 operator/(float b, const Vector3& v) {
    return Vector3(b / v.x, b / v.y, b / v.z);
}

inline void operator/=(float b, Vector3& v) {
    v.x = b / v.x; v.y = b / v.y; v.z = b / v.z;
}

// A few type definitions for vectors to for easier understanding of a purpose in a function for example

// Represents a point for vertices
typedef Vector3 Vertex;
// Represents a point/position in space
typedef Vector3 Point;
// Represents a euler rotation.
// X = pitch, Y = yaw, Z = roll 
typedef Vector3 Euler;
// Represents a scale
typedef Vector3 Size;
// Represents an axis or direction
// In this world, Y = up, Z = forward, X = Left in terms of the direction which is positive
typedef Vector3 Axis;

//Debug printing
//inline std::ostream& operator<<(std::ostream& out, const Vector3& v) {
//    out << v.x << ", " << v.y << ", " << v.z;
//    return out;
//}
//inline std::ostream& operator<<(std::ostream& out, const Vector4& v) {
//    out << v.x << ", " << v.y << ", " << v.z << ", " << v.w;
//    return out;
//}
//
//inline std::ostream& operator<<(std::ostream& out, const Matrix4& m) {
//    out << m.m[0] << ", " << m.m[1] << ", " << m.m[2] << ", " << m.m[3] << "\n";
//    out << m.m[4] << ", " << m.m[5] << ", " << m.m[6] << ", " << m.m[7] << "\n";
//    out << m.m[8] << ", " << m.m[9] << ", " << m.m[10] << ", " << m.m[11] << "\n";
//    out << m.m[12] << ", " << m.m[13] << ", " << m.m[14] << ", " << m.m[15];
//    return out;
//}
