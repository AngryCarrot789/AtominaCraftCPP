#pragma once
#include <cmath>
#include <cfloat>
#include <cassert>
#include "Maths.h"
class Vector2 {
public:
    Vector2() : y(0), x(0) {}
    explicit Vector2(float b) : x(b), y(b) {}
    explicit Vector2(const float* b) : x(b[0]), y(b[1]) {}
    explicit Vector2(float x, float y) : x(x), y(y) {}

    //General
    inline static Vector2 Zero() { return Vector2(0.0f); }
    inline static Vector2 Ones() { return Vector2(1.0f); }
    inline static Vector2 UnitX() { return Vector2(1, 0); }
    inline static Vector2 UnitY() { return Vector2(0, 1); }
    inline static Vector2 UnitZ() { return Vector2(0, 0); }
    inline static Vector2 Up() { return Vector2(0, 1); }
    inline static Vector2 Down() { return Vector2(0, -1); }
    inline static Vector2 Left() { return Vector2(1, 0); }
    inline static Vector2 Right() { return Vector2(-1, 0); }

    //Setters
    inline void Set(float _x, float _y) { x = _x; y = _y; }
    inline void SetZero() { x = 0.0f; y = 0.0f;}
    inline void SetOnes() { x = 1.0f; y = 1.0f;}
    inline void SetUnitX() { x = 1.0f; y = 0.0f; }
    inline void SetUnitY() { x = 0.0f; y = 1.0f; }

    //Basic operatios
    inline Vector2 operator+(float b) const {
        return Vector2(x + b, y + b);
    }
    inline Vector2 operator-(float b) const {
        return Vector2(x - b, y - b);
    }
    inline Vector2 operator*(float b) const {
        return Vector2(x * b, y * b);
    }
    inline Vector2 operator/(float b) const {
        return Vector2(x / b, y / b);
    }
    inline Vector2 operator+(const Vector2& b) const {
        return Vector2(x + b.x, y + b.y);
    }
    inline Vector2 operator-(const Vector2& b) const {
        return Vector2(x - b.x, y - b.y);
    }
    inline Vector2 operator*(const Vector2& b) const {
        return Vector2(x * b.x, y * b.y);
    }
    inline Vector2 operator/(const Vector2& b) const {
        return Vector2(x / b.x, y / b.y);
    }
    inline void operator+=(float b) {
        x += b; y += b;
    }
    inline void operator-=(float b) {
        x -= b; y -= b; 
    }
    inline void operator*=(float b) {
        x *= b; y *= b; 
    }
    inline void operator/=(float b) {
        x /= b; y /= b; 
    }
    inline void operator+=(const Vector2& b) {
        x += b.x; y += b.y;
    }
    inline void operator-=(const Vector2& b) {
        x -= b.x; y -= b.y;
    }
    inline void operator*=(const Vector2& b) {
        x *= b.x; y *= b.y;
    }
    inline void operator/=(const Vector2& b) {
        x /= b.x; y /= b.y;
    }
    inline Vector2 operator-() const {
        return Vector2(-x, -y);
    }

    //Vector algebra
    inline float Dot(const Vector2& b) const {
        return x * b.x + y * b.y;
    }
    inline Vector2 Cross(const Vector2& b) const {
        return Vector2(b.y, -b.x);
    }
    inline float MagSq() const {
        return x * x + y * y;
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
    inline Vector2 Normalized() const {
        return (*this) / Mag();
    }
    inline Vector2 NormalizedSafe() const {
        return (*this) / (Mag() + FLT_EPSILON);
    }
    inline float Angle(const Vector2& b) const {
        return std::acos(Normalized().Dot(b.Normalized()));
    }
    inline float AngleSafe(const Vector2& b) const {
        return std::acos(NormalizedSafe().Dot(b.NormalizedSafe()));
    }
    inline void ClampMagnitude(float m) {
        assert(m > 0.0f);
        const float r = MagSq() / (m * m);
        if (r > 1.0f) {
            (*this) /= std::sqrt(r);
        }
    }

    inline Vector2 DegreesToRadians() {
        return *this / Maths::DEG_RAD_CONST;
    }
    inline Vector2 RadiansToDegrees() {
        return *this * Maths::DEG_RAD_CONST;
    }

    //Other
    inline bool IsNDC() const {
        return (x > -1.0f && x < 1.0f && y > -1.0f && y < 1.0f);
    }

    //Components
    float x, y;
};

// Represents a point for vertices
typedef Vector2 Vertex2;
// Represents a point/position in space
typedef Vector2 Point2;
// Represents a euler rotation.
// X = pitch, Y = yaw
typedef Vector2 Euler2;
// Represents a scale
typedef Vector2 Size2;
// Represents an axis or direction
// In this world, Y = up, X = Left in terms of the direction which is positive
typedef Vector2 Axis2;