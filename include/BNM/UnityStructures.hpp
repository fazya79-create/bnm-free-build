#pragma once

#include "Debug.hpp"
#include "Il2CppStructures.hpp"
namespace BNM {
namespace Structures {
namespace Unity {

struct Vector3;
struct Vector4;
struct Color;
struct Matrix4x4;

struct Vector2 {
    float x;
    float y;
    inline Vector2() : x(0), y(0) {}
    inline Vector2(float x, float y) : x(x), y(y) {}
    inline operator Vector3() const;
    static const Vector2 positiveInfinity;
    static const Vector2 negativeInfinity;
    static const Vector2 down;
    static const Vector2 left;
    static const Vector2 one;
    static const Vector2 right;
    static const Vector2 up;
    static const Vector2 zero;
};

struct Vector3 {
    float x;
    float y;
    float z;
    inline Vector3() : x(0), y(0), z(0) {}
    inline Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    inline operator Vector4() const;
    static const Vector3 positiveInfinity;
    static const Vector3 negativeInfinity;
    static const Vector3 back;
    static const Vector3 down;
    static const Vector3 forward;
    static const Vector3 left;
    static const Vector3 one;
    static const Vector3 right;
    static const Vector3 up;
    static const Vector3 zero;
};

struct Vector4 {
    float x;
    float y;
    float z;
    float w;
    inline Vector4() : x(0), y(0), z(0), w(0) {}
    inline Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    inline Vector4(Color c);
    static const Vector4 positiveInfinity;
    static const Vector4 negativeInfinity;
    static const Vector4 zero;
    static const Vector4 one;
};

struct Color {
    float r;
    float g;
    float b;
    float a;
    inline Color() : r(0), g(0), b(0), a(1) {}
    inline Color(float r, float g, float b, float a = 1) : r(r), g(g), b(b), a(a) {}
    inline Color(Vector4 v) : r(v.x), g(v.y), b(v.z), a(v.w) {}
    static const Color black;
    static const Color red;
    static const Color green;
    static const Color blue;
    static const Color white;
    static const Color orange;
    static const Color yellow;
    static const Color cyan;
    static const Color magenta;
};

struct Color32 {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    inline Color32() : r(0), g(0), b(0), a(255) {}
    inline Color32(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
};

struct Ray {
    Vector3 m_Origin;
    Vector3 m_Direction;
};

struct Quaternion {
    float x;
    float y;
    float z;
    float w;
    inline Quaternion() : x(0), y(0), z(0), w(1) {}
    inline Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    static const Quaternion identity;
};

struct Matrix3x3 {
    float m_Data[9];
    inline Matrix3x3() { memset(m_Data, 0, sizeof(m_Data)); }
    Matrix3x3(const Matrix4x4 &other);
    Matrix3x3 &operator=(const Matrix4x4 &other);
    Matrix3x3 &operator*=(const Matrix4x4 &inM);
    bool Invert();
    inline float *GetPtr() { return m_Data; }
    inline const float *GetPtr() const { return m_Data; }
    inline float &Get(int row, int col) { return m_Data[row * 3 + col]; }
};

struct Matrix4x4 {
    float m_Data[16];
    enum InitIdentity { kIdentity };
    inline Matrix4x4() { memset(m_Data, 0, sizeof(m_Data)); }
    inline Matrix4x4(InitIdentity) { memset(m_Data, 0, sizeof(m_Data)); m_Data[0] = m_Data[5] = m_Data[10] = m_Data[15] = 1; }
    inline float *GetPtr() { return m_Data; }
    inline const float *GetPtr() const { return m_Data; }
    inline float Get(int row, int col) const { return m_Data[row * 4 + col]; }
    static const Matrix4x4 identity;
};

struct RaycastHit {
    Vector3 m_Point;
    Vector3 m_Normal;
    uint32_t m_FaceID;
    float m_Distance;
    Vector2 m_UV;
    int32_t m_Collider;
    void *GetCollider() const;
};

struct RaycastHit2D {
    Vector2 m_Centroid;
    Vector2 m_Point;
    Vector2 m_Normal;
    float m_Distance;
    float m_Fraction;
    int32_t m_Collider;
    void *GetCollider() const;
};

}

}
}
