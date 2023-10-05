#include "..\include\ZeroMath.h"
//
// Created by EunwooSong on 2020-11-30.
//

#include "stdafx.h"
#include "ZeroMath.h"


//////////////////// ZERO_WOP_VECTOR2 ////////////////////
ZeroVec2 ZeroVec2::zero = ZeroVec2(0.0f, 0.0f);
ZeroVec2 ZeroVec2::up = ZeroVec2(0.0f, 1.0f);
ZeroVec2 ZeroVec2::down = ZeroVec2(0.0f, -1.0f);
ZeroVec2 ZeroVec2::left = ZeroVec2(-1.0f, 0.0f);
ZeroVec2 ZeroVec2::right = ZeroVec2(1.0f, 0.0f);
ZeroVec2 ZeroVec2::one = ZeroVec2(1.0f, 1.0f);

/// - = MEMBER FUNC = - ///
float ZeroVec2::Length() {
    return sqrtf(x*x + y*y);
}

float ZeroVec2::LengthSquared() {
    return x*x + y*y;
}

float ZeroVec2::Dot(const ZeroVec2 &v) {
    return x * v.x + y * v.y;
}

ZeroVec2 ZeroVec2::Normalize() {
    ZeroVec2 tmp = *this;

    return tmp /= Length();
}

/// - = STATIC FUNC = - ///
ZeroVec2 ZeroVec2::Clamp(const ZeroVec2 &min, const ZeroVec2 &max, ZeroVec2 &result) {
    (result.x < min.x) ? (result.x = min.x) : ((result.x > max.x) ? (result.x = max.x) : (result.x));
    (result.y < min.y) ? (result.y = min.y) : ((result.y > max.y) ? (result.y = max.y) : (result.y));

    return result;
}

float ZeroVec2::Dot(const ZeroVec2 &v1, const ZeroVec2 &v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

float ZeroVec2::Distance(const ZeroVec2 &v1, const ZeroVec2 &v2) {
    return (v2 - v1).Length();
}

float ZeroVec2::DistanceSquared(const ZeroVec2 &v1, const ZeroVec2 &v2) {
    return (v2 - v1).LengthSquared();
}

ZeroVec2 ZeroVec2::Min(const ZeroVec2 &v1, const ZeroVec2 &v2) {
    ZeroVec2 tmp;
    v1.x <= v2.x ? tmp.x = v1.x : tmp.x = v2.x;
    v1.y <= v2.y ? tmp.y = v1.y : tmp.y = v2.y;
    return tmp;
}

ZeroVec2 ZeroVec2::Max(const ZeroVec2 &v1, const ZeroVec2 &v2) {
    ZeroVec2 tmp;
    v1.x >= v2.x ? tmp.x = v1.x : tmp.x = v2.x;
    v1.y >= v2.y ? tmp.y = v1.y : tmp.y = v2.y;
    return tmp;
}

ZeroVec2 ZeroVec2::Lerp(const ZeroVec2 &v1, const ZeroVec2 &v2, float t) {
    t = ZeroMath::Clamp<float>(t, 1.0f, 0.0f);
    return v1 + (v2 - v1) * t;
}

ZeroVec2 ZeroVec2::SmoothStep(const ZeroVec2 &v1, const ZeroVec2 &v2, float t) {
    t = ZeroMath::Clamp<float>(t, 1.0f, 0.0f);
    t = t * t * (3.0f - 2.0f * t);
    return v1 + (v2 - v1) * t;
}

ZeroVec2 ZeroVec2::Reflect(const ZeroVec2 &iV, const ZeroVec2 &nV) {
    return ZeroVec2(glm::reflect((glm::vec2)iV, (glm::vec2)nV));
}

ZeroVec2 ZeroVec2::Sub(const ZeroVec2 &left, const ZeroVec2 &right) {
    return ZeroVec2(left.x - right.x, left.y - right.y);
}


/// - = VEC2 Operators = - ///
ZeroVec2 &ZeroVec2::operator+=(const ZeroVec2 & rh) {
    this->x += rh.x;
    this->y += rh.y;
    return *this;
}

ZeroVec2 &ZeroVec2::operator-=(const ZeroVec2 & rh) {
    this->x -= rh.x;
    this->y -= rh.y;
    return *this;
}

ZeroVec2 &ZeroVec2::operator*=(float rh) {
    this->x *= rh;
    this->y *= rh;
    return *this;
}

ZeroVec2 &ZeroVec2::operator/=(float rh) {
    this->x /= rh;
    this->y /= rh;
    return *this;
}

ZeroVec2 ZeroVec2::operator+() const {
    return ZeroVec2(x, y);
}

ZeroVec2 ZeroVec2::operator-() const {
    return ZeroVec2(-x, -y);
}

ZeroVec2 ZeroVec2::operator+(const ZeroVec2 & rh) const {
    return ZeroVec2(x + rh.x, y + rh.y);
}

ZeroVec2 ZeroVec2::operator-(const ZeroVec2 & rh) const {
    return ZeroVec2(x - rh.x, y - rh.y);
}

ZeroVec2 ZeroVec2::operator*(float rh) const {
    return  ZeroVec2(x * rh, y * rh);
}

ZeroVec2 ZeroVec2::operator/(float rh) const {
    return ZeroVec2(x / rh, y / rh);
}

ZeroVec2 operator*(float lh, const ZeroVec2 & rh) {
    return lh * (glm::vec2)rh;
}

bool ZeroVec2::operator==(const ZeroVec2 & r) const {
    return x == r.x && y == r.y;
}

bool ZeroVec2::operator!=(const ZeroVec2 &r) const {
    return x != r.x || y != r.y;
}

ZeroVec2 ZeroVec2::Cross(ZeroVec2& v, float s) {
    return ZeroVec2(s * v.y, -s * v.x);
}

float ZeroVec2::Cross(ZeroVec2& lv, ZeroVec2& rv) {
    return lv.x * rv.y - lv.y * rv.x;
}

ZeroVec2 ZeroVec2::Cross(float v, ZeroVec2& s) {
    return Vec2(v * s.y, -v * s.x);
}

ZeroVec2 ZeroVec2::Scale(ZeroVec2& lv, ZeroVec2& rv)
{
    return ZeroVec2(lv.x * rv.x, lv.y * rv.y);
}

ZeroVec2 ZeroVec2::Abs(ZeroVec2 vec2) {
    return ZeroVec2(std::abs(vec2.x), std::abs(vec2.y));
}

void ZeroVec2::Set(double x, double y) {
    this->x = x;
    this->y = y;
}

//ZeroMat22 ZeroMat22::operator+(const ZeroMat22 & rh) const {
//    return Mat22(col1 + rh.col1, col2 + rh.col2);
//}
//
//ZeroMat22 ZeroMat22::operator*(const ZeroMat22 & rh) const {
//    return ZeroMat22((*this) * rh.col1, (*this) * rh.col2);
//}
