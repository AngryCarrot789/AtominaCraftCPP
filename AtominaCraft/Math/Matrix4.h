#pragma once
#include <cmath>
#include <memory>
#include "Vector3.h"
#include "Vector4.h"
class Matrix4 {
public:
    //Constructors
    Matrix4() {}
    explicit Matrix4(float b) { Fill(b); }

    //General
    inline void Fill(float b) {
        std::fill(m, m + 16, b);
    }
    inline void MakeZero() {
        Fill(0.0f);
    }
    inline void MakeIdentity() {
        m[0] = 1.0f; m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f;
        m[4] = 0.0f; m[5] = 1.0f; m[6] = 0.0f; m[7] = 0.0f;
        m[8] = 0.0f; m[9] = 0.0f; m[10] = 1.0f; m[11] = 0.0f;
        m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
    }
    inline void MakeRotationX(float a) {
        float sin = std::sin(a);
        float cos = std::cos(a);
        m[0] = 1.0f; m[1] = 0.0f;   m[2] = 0.0f;   m[3] = 0.0f;
        m[4] = 0.0f; m[5] = cos;    m[6] = -sin;   m[7] = 0.0f;
        m[8] = 0.0f; m[9] = sin;    m[10] = cos;   m[11] = 0.0f;
        m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f;  m[15] = 1.0f;
    }
    inline void MakeRotationY(float a) {
        float sin = std::sin(a);
        float cos = std::cos(a);
        m[0] = cos;   m[1] = 0.0f;  m[2] = sin;   m[3] = 0.0f;
        m[4] = 0.0f;  m[5] = 1.0f;  m[6] = 0.0f;  m[7] = 0.0f;
        m[8] = -sin;  m[9] = 0.0f;  m[10] = cos;  m[11] = 0.0f;
        m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
    }
    inline void MakeRotationZ(float a) {
        float sin = std::sin(a);
        float cos = std::cos(a);
        m[0] = cos;   m[1] = -sin;  m[2] = 0.0f;  m[3] = 0.0f;
        m[4] = sin;   m[5] = cos;   m[6] = 0.0f;  m[7] = 0.0f;
        m[8] = 0.0f;  m[9] = 0.0f;  m[10] = 1.0f; m[11] = 0.0f;
        m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
    }
    inline void MakeTranslation(const Vector3& t) {
        m[0] = 1.0f; m[1] = 0.0f; m[2] = 0.0f; m[3] = t.x;
        m[4] = 0.0f; m[5] = 1.0f; m[6] = 0.0f; m[7] = t.y;
        m[8] = 0.0f; m[9] = 0.0f; m[10] = 1.0f; m[11] = t.z;
        m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
    }
    inline void MakeScale(const Vector3& s) {
        m[0] = s.x;  m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f;
        m[4] = 0.0f; m[5] = s.y;  m[6] = 0.0f; m[7] = 0.0f;
        m[8] = 0.0f; m[9] = 0.0f; m[10] = s.z;  m[11] = 0.0f;
        m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
    }

    //Statics
    inline static Matrix4 Zero() { Matrix4 m; m.MakeZero(); return m; }
    inline static Matrix4 Identity() { Matrix4 m; m.MakeIdentity(); return m; }
    inline static Matrix4 RotatedX(float a) { Matrix4 m; m.MakeRotationX(a); return m; }
    inline static Matrix4 RotatedY(float a) { Matrix4 m; m.MakeRotationY(a); return m; }
    inline static Matrix4 RotatedZ(float a) { Matrix4 m; m.MakeRotationZ(a); return m; }
    inline static Matrix4 Translated(const Vector3& t) { Matrix4 m; m.MakeTranslation(t); return m; }
    inline static Matrix4 Scaled(float s) { Matrix4 m; m.MakeScale(Vector3(s)); return m; }
    inline static Matrix4 Scaled(const Vector3& s) { Matrix4 m; m.MakeScale(s); return m; }

    inline static Matrix4 CreateLocalToWorld(Point pos, Euler euler, Size scale) {
        return
            Matrix4::Translated(pos) *
            Matrix4::RotatedY(euler.y) *
            Matrix4::RotatedX(euler.x) *
            Matrix4::RotatedZ(euler.z) *
            Matrix4::Scaled(scale);
    }

    inline static Matrix4 CreateWorldToLocal(Point pos, Euler euler, Size scale) {
        return
            Matrix4::Scaled(1.0f / scale) *
            Matrix4::RotatedZ(-euler.z) *
            Matrix4::RotatedX(-euler.x) *
            Matrix4::RotatedY(-euler.y) *
            Matrix4::Translated(-pos);
    }
    inline static Matrix4 CreateProjection(int w, int h, float fov, float n, float f) {
        const float fovRads = 1.0f / std::tan(fov * Maths::PI / 360.0f);
        const float aspect = ((float)h) / ((float)w);
        const float distance = n - f;

        Matrix4 projection;
        projection.m[0] = fovRads * aspect;
        projection.m[1] = 0.0f;
        projection.m[2] = 0.0f;
        projection.m[3] = 0.0f;

        projection.m[4] = 0.0f;
        projection.m[5] = fovRads;
        projection.m[6] = 0.0f;
        projection.m[7] = 0.0f;

        projection.m[8] = 0.0f;
        projection.m[9] = 0.0f;
        projection.m[10] = (n + f) / distance;
        projection.m[11] = (2 * n * f) / distance;

        projection.m[12] = 0.0f;
        projection.m[13] = 0.0f;
        projection.m[14] = -1.0f;
        projection.m[15] = 0.0f;
        return projection;
    }

    //Some getters
    inline Axis XAxis() const {
        return Vector3(m[0], m[4], m[8]);
    }
    inline Axis YAxis() const {
        return Vector3(m[1], m[5], m[9]);
    }
    inline Axis ZAxis() const {
        return Vector3(m[2], m[6], m[10]);
    }
    inline Point Translation() const {
        return Vector3(m[3], m[7], m[11]);
    }
    inline Size Scaled() const {
        return Vector3(m[0], m[5], m[10]);
    }

    //Setters
    inline void SetTranslation(const Vector3& t) {
        m[3] = t.x;
        m[7] = t.y;
        m[11] = t.z;
    }
    inline void SetXAxis(const Vector3& t) {
        m[0] = t.x;
        m[4] = t.y;
        m[8] = t.z;
    }
    inline void SetYAxis(const Vector3& t) {
        m[1] = t.x;
        m[5] = t.y;
        m[9] = t.z;
    }
    inline void SetZAxis(const Vector3& t) {
        m[2] = t.x;
        m[6] = t.y;
        m[10] = t.z;
    }
    inline void SetScale(const Vector3& s) {
        m[0] = s.x;
        m[5] = s.y;
        m[10] = s.z;
    }

    //Transformations
    inline Matrix4 Transposed() const {
        Matrix4 out;
        out.m[0] = m[0]; out.m[1] = m[4]; out.m[2] = m[8];  out.m[3] = m[12];
        out.m[4] = m[1]; out.m[5] = m[5]; out.m[6] = m[9];  out.m[7] = m[13];
        out.m[8] = m[2]; out.m[9] = m[6]; out.m[10] = m[10]; out.m[11] = m[14];
        out.m[12] = m[3]; out.m[13] = m[7]; out.m[14] = m[11]; out.m[15] = m[15];
        return out;
    }
    inline void Translate(const Vector3& t) {
        m[3] += t.x;
        m[7] += t.y;
        m[11] += t.z;
    }
    inline void Stretch(const Vector3& s) {
        m[0] *= s.x;
        m[5] *= s.y;
        m[10] *= s.z;
    }

    //Basic operatios
    inline Matrix4 operator+(const Matrix4& b) const {
        Matrix4 out;
        for (int i = 0; i < 16; ++i) { out.m[i] = m[i] + b.m[i]; }
        return out;
    }
    inline Matrix4 operator-(const Matrix4& b) const {
        Matrix4 out;
        for (int i = 0; i < 16; ++i) { out.m[i] = m[i] - b.m[i]; }
        return out;
    }
    inline void operator+=(const Matrix4& b) {
        for (int i = 0; i < 16; ++i) { m[i] += b.m[i]; }
    }
    inline void operator-=(const Matrix4& b) {
        for (int i = 0; i < 16; ++i) { m[i] -= b.m[i]; }
    }
    inline void operator*=(float b) {
        for (int i = 0; i < 16; ++i) { m[i] *= b; }
    }
    inline void operator/=(float b) {
        operator*=(1.0f / b);
    }

    //Multiplication
    Matrix4 operator*(const Matrix4& b) const {
        Matrix4 out;
        out.m[0] = b.m[0] * m[0] + b.m[4] * m[1] + b.m[8] * m[2] + b.m[12] * m[3];
        out.m[1] = b.m[1] * m[0] + b.m[5] * m[1] + b.m[9] * m[2] + b.m[13] * m[3];
        out.m[2] = b.m[2] * m[0] + b.m[6] * m[1] + b.m[10] * m[2] + b.m[14] * m[3];
        out.m[3] = b.m[3] * m[0] + b.m[7] * m[1] + b.m[11] * m[2] + b.m[15] * m[3];

        out.m[4] = b.m[0] * m[4] + b.m[4] * m[5] + b.m[8] * m[6] + b.m[12] * m[7];
        out.m[5] = b.m[1] * m[4] + b.m[5] * m[5] + b.m[9] * m[6] + b.m[13] * m[7];
        out.m[6] = b.m[2] * m[4] + b.m[6] * m[5] + b.m[10] * m[6] + b.m[14] * m[7];
        out.m[7] = b.m[3] * m[4] + b.m[7] * m[5] + b.m[11] * m[6] + b.m[15] * m[7];

        out.m[8] = b.m[0] * m[8] + b.m[4] * m[9] + b.m[8] * m[10] + b.m[12] * m[11];
        out.m[9] = b.m[1] * m[8] + b.m[5] * m[9] + b.m[9] * m[10] + b.m[13] * m[11];
        out.m[10] = b.m[2] * m[8] + b.m[6] * m[9] + b.m[10] * m[10] + b.m[14] * m[11];
        out.m[11] = b.m[3] * m[8] + b.m[7] * m[9] + b.m[11] * m[10] + b.m[15] * m[11];

        out.m[12] = b.m[0] * m[12] + b.m[4] * m[13] + b.m[8] * m[14] + b.m[12] * m[15];
        out.m[13] = b.m[1] * m[12] + b.m[5] * m[13] + b.m[9] * m[14] + b.m[13] * m[15];
        out.m[14] = b.m[2] * m[12] + b.m[6] * m[13] + b.m[10] * m[14] + b.m[14] * m[15];
        out.m[15] = b.m[3] * m[12] + b.m[7] * m[13] + b.m[11] * m[14] + b.m[15] * m[15];
        return out;
    }
    void operator*=(const Matrix4& b) {
        (*this) = operator*(b);
    }
    Vector4 operator*(const Vector4& b) const {
        return Vector4(
            m[0] * b.x + m[1] * b.y + m[2] * b.z + m[3] * b.w,
            m[4] * b.x + m[5] * b.y + m[6] * b.z + m[7] * b.w,
            m[8] * b.x + m[9] * b.y + m[10] * b.z + m[11] * b.w,
            m[12] * b.x + m[13] * b.y + m[14] * b.z + m[15] * b.w
        );
    }
    Point MultiplyPoint(const Vector3& b) const {
        const Vector3 p(
            m[0] * b.x + m[1] * b.y + m[2] * b.z + m[3],
            m[4] * b.x + m[5] * b.y + m[6] * b.z + m[7],
            m[8] * b.x + m[9] * b.y + m[10] * b.z + m[11]
        );
        const float w = m[12] * b.x + m[13] * b.y + m[14] * b.z + m[15];
        return p / w;
    }
    Euler MulDirection(const Vector3& b) const {
        return Vector3(
            m[0] * b.x + m[1] * b.y + m[2] * b.z,
            m[4] * b.x + m[5] * b.y + m[6] * b.z,
            m[8] * b.x + m[9] * b.y + m[10] * b.z
        );
    }

    //Inverse
    Matrix4 Inverse() const {
        Matrix4 inv;

        inv.m[0] = m[5] * m[10] * m[15] -
            m[5] * m[11] * m[14] -
            m[9] * m[6] * m[15] +
            m[9] * m[7] * m[14] +
            m[13] * m[6] * m[11] -
            m[13] * m[7] * m[10];

        inv.m[4] = -m[4] * m[10] * m[15] +
            m[4] * m[11] * m[14] +
            m[8] * m[6] * m[15] -
            m[8] * m[7] * m[14] -
            m[12] * m[6] * m[11] +
            m[12] * m[7] * m[10];

        inv.m[8] = m[4] * m[9] * m[15] -
            m[4] * m[11] * m[13] -
            m[8] * m[5] * m[15] +
            m[8] * m[7] * m[13] +
            m[12] * m[5] * m[11] -
            m[12] * m[7] * m[9];

        inv.m[12] = -m[4] * m[9] * m[14] +
            m[4] * m[10] * m[13] +
            m[8] * m[5] * m[14] -
            m[8] * m[6] * m[13] -
            m[12] * m[5] * m[10] +
            m[12] * m[6] * m[9];

        inv.m[1] = -m[1] * m[10] * m[15] +
            m[1] * m[11] * m[14] +
            m[9] * m[2] * m[15] -
            m[9] * m[3] * m[14] -
            m[13] * m[2] * m[11] +
            m[13] * m[3] * m[10];

        inv.m[5] = m[0] * m[10] * m[15] -
            m[0] * m[11] * m[14] -
            m[8] * m[2] * m[15] +
            m[8] * m[3] * m[14] +
            m[12] * m[2] * m[11] -
            m[12] * m[3] * m[10];

        inv.m[9] = -m[0] * m[9] * m[15] +
            m[0] * m[11] * m[13] +
            m[8] * m[1] * m[15] -
            m[8] * m[3] * m[13] -
            m[12] * m[1] * m[11] +
            m[12] * m[3] * m[9];

        inv.m[13] = m[0] * m[9] * m[14] -
            m[0] * m[10] * m[13] -
            m[8] * m[1] * m[14] +
            m[8] * m[2] * m[13] +
            m[12] * m[1] * m[10] -
            m[12] * m[2] * m[9];

        inv.m[2] = m[1] * m[6] * m[15] -
            m[1] * m[7] * m[14] -
            m[5] * m[2] * m[15] +
            m[5] * m[3] * m[14] +
            m[13] * m[2] * m[7] -
            m[13] * m[3] * m[6];

        inv.m[6] = -m[0] * m[6] * m[15] +
            m[0] * m[7] * m[14] +
            m[4] * m[2] * m[15] -
            m[4] * m[3] * m[14] -
            m[12] * m[2] * m[7] +
            m[12] * m[3] * m[6];

        inv.m[10] = m[0] * m[5] * m[15] -
            m[0] * m[7] * m[13] -
            m[4] * m[1] * m[15] +
            m[4] * m[3] * m[13] +
            m[12] * m[1] * m[7] -
            m[12] * m[3] * m[5];

        inv.m[14] = -m[0] * m[5] * m[14] +
            m[0] * m[6] * m[13] +
            m[4] * m[1] * m[14] -
            m[4] * m[2] * m[13] -
            m[12] * m[1] * m[6] +
            m[12] * m[2] * m[5];

        inv.m[3] = -m[1] * m[6] * m[11] +
            m[1] * m[7] * m[10] +
            m[5] * m[2] * m[11] -
            m[5] * m[3] * m[10] -
            m[9] * m[2] * m[7] +
            m[9] * m[3] * m[6];

        inv.m[7] = m[0] * m[6] * m[11] -
            m[0] * m[7] * m[10] -
            m[4] * m[2] * m[11] +
            m[4] * m[3] * m[10] +
            m[8] * m[2] * m[7] -
            m[8] * m[3] * m[6];

        inv.m[11] = -m[0] * m[5] * m[11] +
            m[0] * m[7] * m[9] +
            m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] -
            m[8] * m[1] * m[7] +
            m[8] * m[3] * m[5];

        inv.m[15] = m[0] * m[5] * m[10] -
            m[0] * m[6] * m[9] -
            m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] +
            m[8] * m[1] * m[6] -
            m[8] * m[2] * m[5];

        const float det = m[0] * inv.m[0] + m[1] * inv.m[4] + m[2] * inv.m[8] + m[3] * inv.m[12];
        inv /= det;
        return inv;
    }

    //Components
    float m[16];
};