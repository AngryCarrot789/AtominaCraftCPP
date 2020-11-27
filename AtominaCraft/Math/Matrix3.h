#pragma once
#include <cmath>
#include <memory>
#include "Vector2.h"
class Matrix3 {
public:
    //Constructors
    Matrix3() { Fill(0.0f); }
    explicit Matrix3(float b) { Fill(b); }

    //General
    inline void Fill(float b) {
        std::fill(m, m + 9, b);
    }
    inline void MakeZero() {
        Fill(0.0f);
    }
    inline void MakeIdentity() {
        m[0] = 1.0f; m[1] = 0.0f; m[2] = 0.0f; 
        m[3] = 0.0f; m[4] = 1.0f; m[5] = 0.0f; 
        m[6] = 0.0f; m[7] = 0.0f; m[8] = 1.0f; 
    }
    inline void MakeRotation(float a) {
        float sin = std::sin(a);
        float cos = std::cos(a);
        m[0] = cos;  m[1] = -sin; m[2] = 0.0f;
        m[3] = sin;  m[4] = cos;  m[5] = 0.0f;
        m[6] = 0.0f; m[7] = 0.0f; m[8] = 1.0f;
    }
    inline void MakeTranslation(const Vector2& t) {
        m[0] = 1.0f; m[1] = 0.0f; m[2] = t.x;
        m[3] = 0.0f; m[4] = 1.0f; m[5] = t.y;
        m[6] = 0.0f; m[7] = 0.0f; m[8] = 1.0f;
    }
    inline void MakeScale(const Vector2& s) {
        m[0] = s.x;  m[1] = 0.0f; m[2] = 0.0f;
        m[3] = 0.0f; m[4] = s.y;  m[5] = 0.0f;
        m[6] = 0.0f; m[7] = 0.0f; m[8] = 1.0f; 
    }

    //Statics
    inline static Matrix3 Zero() { Matrix3 m; m.MakeZero(); return m; }
    inline static Matrix3 Identity() { Matrix3 m; m.MakeIdentity(); return m; }
    inline static Matrix3 Rotated(float a) { Matrix3 m; m.MakeRotation(a); return m; }
    inline static Matrix3 Translated(const Vector2& t) { Matrix3 m; m.MakeTranslation(t); return m; }
    inline static Matrix3 Scaled(float s) { Matrix3 m; m.MakeScale(Vector2(s)); return m; }
    inline static Matrix3 Scaled(const Vector2& s) { Matrix3 m; m.MakeScale(s); return m; }

    //Some getters
    inline Axis2 XAxis() const {
        return Vector2(m[0], m[3]);
    }
    inline Axis2 YAxis() const {
        return Vector2(m[1], m[4]);
    }
    inline Point2 Translation() const {
        return Vector2(m[2], m[5]);
    }
    inline Size2 Scaled() const {
        return Vector2(m[0], m[4]);
    }

    //Setters
    inline void SetTranslation(const Vector2& t) {
        m[2] = t.x;
        m[5] = t.y;
    }
    inline void SetXAxis(const Vector2& t) {
        m[0] = t.x;
        m[3] = t.y;
    }
    inline void SetYAxis(const Vector2& t) {
        m[1] = t.x;
        m[4] = t.y;
    }
    inline void SetScale(const Vector2& s) {
        m[0] = s.x;
        m[4] = s.y;
    }

    //Transformations
    inline void Translate(const Vector2& t) {
        m[2] += t.x;
        m[5] += t.y;
    }
    inline void Stretch(const Vector2& s) {
        m[0] *= s.x;
        m[4] *= s.y;
    }

    //Basic operatios
    inline Matrix3 operator+(const Matrix3& b) const {
        Matrix3 out;
        for (int i = 0; i < 9; ++i) { out.m[i] = m[i] + b.m[i]; }
        return out;
    }
    inline Matrix3 operator-(const Matrix3& b) const {
        Matrix3 out;
        for (int i = 0; i < 9; ++i) { out.m[i] = m[i] - b.m[i]; }
        return out;
    }
    inline void operator+=(const Matrix3& b) {
        for (int i = 0; i < 9; ++i) { m[i] += b.m[i]; }
    }
    inline void operator-=(const Matrix3& b) {
        for (int i = 0; i < 9; ++i) { m[i] -= b.m[i]; }
    }
    inline void operator*=(float b) {
        for (int i = 0; i < 9; ++i) { m[i] *= b; }
    }
    inline void operator/=(float b) {
        operator*=(1.0f / b);
    }

    //Multiplication
    Matrix3 operator*(const Matrix3& mat) const {
        Matrix3 result;
        result.m[0] = m[0] * mat.m[0] + m[1] * mat.m[3] + m[2] * mat.m[6];
        result.m[1] = m[0] * mat.m[1] + m[1] * mat.m[4] + m[2] * mat.m[7];
        result.m[2] = m[0] * mat.m[2] + m[1] * mat.m[5] + m[2] * mat.m[8];

        result.m[3] = m[3] * mat.m[0] + m[4] * mat.m[3] + m[5] * mat.m[6];
        result.m[4] = m[3] * mat.m[1] + m[4] * mat.m[4] + m[5] * mat.m[7];
        result.m[5] = m[3] * mat.m[2] + m[4] * mat.m[5] + m[5] * mat.m[8];

        result.m[6] = m[6] * mat.m[0] + m[7] * mat.m[3] + m[8] * mat.m[6];
        result.m[7] = m[6] * mat.m[1] + m[7] * mat.m[4] + m[8] * mat.m[7];
        result.m[8] = m[6] * mat.m[2] + m[7] * mat.m[5] + m[8] * mat.m[8];
        return result;
    }
    void operator*=(const Matrix3& b) {
        (*this) = operator*(b);
    }
    Point2 MultiplyPoint(const Point2& b) const {
        const Point2 p(
            m[0] * b.x + m[1] * b.y + m[2],
            m[3] * b.x + m[4] * b.y + m[5]
        );
        const float w = m[6] * b.x + m[7] * b.y + m[8];
        return p / w;
    }
    Euler2 MulDirection(const Vector2& b) const {
        return Vector2(
            m[0] * b.x + m[1] * b.y,
            m[3] * b.x + m[4] * b.y
        );
    }


    //Components
    float m[9];
};