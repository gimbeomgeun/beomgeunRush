//
// Created by EunwooSong on 2020-11-30.
//

#ifndef ZERO_ENGINE_ZEROMATH_H
#define ZERO_ENGINE_ZEROMATH_H

#include "ZeroLogger.h"

////////// GLM //////////
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include <random>
#include <ctime>

const float pi = 3.14159265358979323846264f;

class ZeroVec2;
class ZeroMat22;

//////////////////// ZERO_WOP_VECTOR2 ////////////////////
typedef class ZeroVec2 :
        public glm::vec<2, double, glm::defaultp> {
public:
    /// - = USEFUL CONSTANTS  = - ///
    static ZeroVec2 zero;
    static ZeroVec2 up;
    static ZeroVec2 down;
    static ZeroVec2 left;
    static ZeroVec2 right;
    static ZeroVec2 one;

    /// - = INITIALIZE VECTOR2 = - ///
    ZeroVec2() { x = 0.0f; y = 0.0f;};
    ~ZeroVec2() = default;
    template<typename T>
    ZeroVec2(T x, T y) : vec() {
        this->x = static_cast<float>(x);
        this->y = static_cast<float>(y);
    }
    ZeroVec2(const ZeroVec2& v)   { x = v.x; y = v.y;};
    ZeroVec2(const glm::vec2& v)     { x = v.x; y = v.y;};

    /// - = VEC2 Operators = - ///
    ZeroVec2& operator+=(const ZeroVec2&);
    ZeroVec2& operator-=(const ZeroVec2&);
    ZeroVec2& operator*=(float);
    ZeroVec2& operator/=(float);

    ZeroVec2 operator+() const;
    ZeroVec2 operator-() const;

    ZeroVec2 operator+(const ZeroVec2&) const;
    ZeroVec2 operator-(const ZeroVec2&) const;
    ZeroVec2 operator*(float) const;
    ZeroVec2 operator/(float) const;

    friend ZeroVec2 operator*(float, const ZeroVec2&);

    bool operator==(const ZeroVec2&) const;
    bool operator!=(const ZeroVec2&) const;

    /// - = MEMBER FUNC = - ///
    float Length();
    float LengthSquared();

    float Dot(const ZeroVec2& v);

    ZeroVec2 Normalize();

    /// - = STATIC FUNC = - ///
    static ZeroVec2 Clamp        (const ZeroVec2& min, const ZeroVec2& max, ZeroVec2& result);

    static float Dot                (const ZeroVec2& v1, const ZeroVec2& v2);
    static float Distance           (const ZeroVec2& v1, const ZeroVec2& v2);
    static float DistanceSquared    (const ZeroVec2& v1, const ZeroVec2& v2);

    static ZeroVec2 Min          (const ZeroVec2& v1, const ZeroVec2& v2);
    static ZeroVec2 Max          (const ZeroVec2& v1, const ZeroVec2& v2);

    static ZeroVec2 Lerp         (const ZeroVec2& v1, const ZeroVec2& v2, float t);
    static ZeroVec2 SmoothStep   (const ZeroVec2& v1, const ZeroVec2& v2, float t);

    static ZeroVec2 Reflect      (const ZeroVec2& iV, const ZeroVec2& nV);
    static ZeroVec2 Sub          (const ZeroVec2& left, const ZeroVec2& right);

    static ZeroVec2 Cross(ZeroVec2& v, float s);
    static ZeroVec2 Cross(float s, ZeroVec2& v);
    static float Cross(ZeroVec2& lv, ZeroVec2& rv);

    static ZeroVec2 Scale(ZeroVec2& lv, ZeroVec2& rv);
    static ZeroVec2 Abs(ZeroVec2 vec2);

    void Set(double x, double y);
} Vec2;

//////////////////// ZERO_WOP_MAT22 ////////////////////
typedef class ZeroMat22 {
public:
    ZeroMat22() : col1(0, 0), col2(0, 0) {}
    ZeroMat22(float angle)
    {
        float c = cosf(angle), s = sinf(angle);
        col1.x = c; col2.x = -s;
        col1.y = s; col2.y = c;
    }

    ZeroMat22(const Vec2& col1, const Vec2& col2) : col1(col1), col2(col2) {}

    ZeroMat22 Transpose() const
    {
        return ZeroMat22(Vec2(col1.x, col2.x), Vec2(col1.y, col2.y));
    }

    ZeroMat22 Invert() const
    {
        float a = col1.x, b = col2.x, c = col1.y, d = col2.y;
        float det = a * d - b * c;

        if(det != 0.0f)
            det = 1.0f / det;
        else {
            ZERO_LOGGER::Error("Zero Exception");
        }

        ZeroMat22 tmp;
        tmp.col1.x =  det * d;	tmp.col2.x = -det * b;
        tmp.col1.y = -det * c;	tmp.col2.y =  det * a;
        return tmp;
    }

    static ZeroMat22 Abs(const ZeroMat22& A)
    {
        return ZeroMat22(Vec2::Abs(A.col1), Vec2::Abs(A.col2));
    }

    //ZeroMat22 operator+(const ZeroMat22&) const;
    //ZeroMat22 operator*(const ZeroMat22&) const;

    Vec2 col1, col2;
} Mat22;

inline Vec2 operator * (const Mat22& A, const Vec2& v)
{
    return Vec2(A.col1.x * v.x + A.col2.x * v.y, A.col1.y * v.x + A.col2.y * v.y);
}

inline Mat22 operator + (const Mat22& A, const Mat22& B)
{
    return Mat22(A.col1 + B.col1, A.col2 + B.col2);
}

inline Mat22 operator * (const Mat22& A, const Mat22& B)
{
    return Mat22(A * B.col1, A * B.col2);
}


//////////////////// ZERO_WOP_UTIL_FUNC ////////////////////
namespace ZeroMath {
    template <class T>
    inline T Max(T v1, T max) {
        return v1 > max ? v1 : max;
    }
    template <typename T>
    inline T Min(T v1, T min) {
        return v1 < min ? v1 : min;
    }
    template<typename T>
    inline T Abs(T a)
    {
        return a > 0.0f ? a : -a;
    }
    template <class T>
    inline T Clamp(T v1, T min, T max) {
        return Max(min, Min(v1, max));
    }

    inline float Sign(double x) {
        return x < 0.0 ? -1.0f : 1.0f;
    }

    inline float Random(float form, float to) {
        static std::default_random_engine random(time(NULL));
        std::uniform_real_distribution<float> d(form, to);
        return d(random);
    }

    inline float Angle(float p1X, float p1Y, float p2X, float p2Y) {
        return atan2(p2Y - p1Y, p2X - p1X );
    }

    //rotationCenter : L - T : Vec2(0.0f, 0.0f) / CENTER : Vec2(0.5f, 0.5f) / R - B : Vec2(1.0f, 1.0f)
    inline glm::mat4 MatrixTransformation(ZeroVec2 rotationCenter, ZeroVec2 scale, float rotation, ZeroVec2 pos) {
        glm::mat4 mat = glm::mat4(1.0f);

        mat = glm::translate(mat, glm::vec3(pos, 0.0f));

        mat = glm::translate(mat, glm::vec3(rotationCenter.x * scale.x, rotationCenter.y * scale.y, 0.0f));
        mat = glm::rotate(mat, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        mat = glm::translate(mat, glm::vec3(-rotationCenter.x * scale.x, -rotationCenter.y * scale.y, 0.0f));

        mat = glm::scale(mat, glm::vec3(scale, 1.0f));

        return mat;
    }

    inline float Dot(const Vec2& a, const Vec2& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    inline float Cross(const Vec2& a, const Vec2& b)
    {
        return a.x * b.y - a.y * b.x;
    }

    inline Vec2 Cross(const Vec2& a, float s)
    {
        return Vec2(s * a.y, -s * a.x);
    }

    inline Vec2 Cross(float s, const Vec2& a)
    {
        return Vec2(-s * a.y, s * a.x);
    }

    inline Vec2 Abs(const Vec2& a)
    {
        return Vec2(std::abs(a.x), std::abs(a.y));
    }

    inline Mat22 Abs(const Mat22& A)
    {
        return Mat22(Abs(A.col1), Abs(A.col2));
    }

    template <class T>
    inline T Lerp(T p1, T p2, float v) {
        v = (v > 1.0f) ? 1.0f : v;
        return p1 + (p2 - p1) * v;
    }
}

#endif //ZERO_ENGINE_ZEROMATH_H
