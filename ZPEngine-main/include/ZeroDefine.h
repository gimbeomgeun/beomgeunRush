//
// Created by HADMARINE on 2020-12-12.
//

#ifndef ZERO_ENGINE_ZEROWOPUTIL_HPP
#define ZERO_ENGINE_ZEROWOPUTIL_HPP

#include <bitset>
#include <cstdint>
#include "ZeroMath.h"

//////////////////// ZERO_WOP_ECS_TYPES ////////////////////
using EntityID = std::uint32_t;
const EntityID MAX_ENTITIES = 1000000;
using ComponentID = std::uint8_t;
const ComponentID MAX_COMPONENTS = 100;
using Signature = std::bitset<MAX_COMPONENTS>;

//////////////////// ZERO_WOP_UTILS ////////////////////
template <typename T> inline void Swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = a;
}

//////////////////// ZERO_WOP_PHYSICS ////////////////////
#define MAX_POINTS 2

union FeaturePair {
    struct Edges {
        char inEdge1;
        char outEdge1;
        char inEdge2;
        char outEdge2;
    } e;
    int value;
};

//충돌 부분 구조체
struct Contact {
    Contact() : Pn(0.0f), Pt(0.0f), Pnb(0.0f) {};

    Vec2 position;
    Vec2 normal;
    Vec2 r1, r2;
    float separation;
    float Pn;   // normal impulse
    float Pt;   // tangent impluse
    float Pnb;  // normal impulse for position bias
    float massNormal, massTangent;
    float bias;
    FeaturePair feature;
};


//Rect
//left : posX, right : posY
class Rect {
public:
    Rect() {
        left = 0;
        right = 0;
        top = 0;
        bottom = 0;
    }

    Rect(float r, float b) {
        left = 0;
        right = r;
        top = 0;
        bottom = b;
    }

    Rect(float l, float t, float r, float b) {
        left = l;
        right = r;
        top = t;
        bottom = b;
    }

    Rect(Vec2 lt, Vec2 rb) {
        left = lt.x;
        top = lt.y;
        right = rb.x;
        bottom = rb.y;
    }

    //Rect Collision
    bool Intersects(Rect r) {
        return left < r.right && top < r.bottom && right > r.left && bottom > r.top;
    }
    static bool Intersects(Rect lv, Rect rv) {
        return lv.left < rv.right && lv.top < rv.bottom && lv.right > rv.left && lv.bottom > rv.top;
    }

    bool Contain(Vec2 p) {
        return left < p.x&& p.x < right&& top < p.y&& p.y < bottom;
    }

    Rect offset(Vec2 p) {
        Rect r = *this;
        r.left += p.x;
        r.right += p.x;
        r.top += p.y;
        r.bottom += p.y;

        return r;
    }

    float width() {
        return right - left;
    }

    float height() {
        return bottom - top;
    }

    Vec2 center() {
        return Vec2(width(), height()) / 2.0f;
    }

    Vec2 Size() {
        return Vec2(right, bottom);
    }

public:
    float left, right, bottom, top;
};

#endif //ZERO_ENGINE_ZEROWOPUTIL_HPP
