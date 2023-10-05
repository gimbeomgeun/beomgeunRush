//
// Created by EunwooSong on 2020-12-12.
//

#ifndef ZERO_ENGINE_ZEROCOLLIDERMANAGER_H
#define ZERO_ENGINE_ZEROCOLLIDERMANAGER_H

#include <vector>
#include <unordered_set>
#include <map>
#include "ZeroMath.h"
#include "ZeroDefine.h"
#include "ZeroColliderGroup.h"

class RigidBody2D;
class Joint;

enum Axis
{
    FACE_A_X,
    FACE_A_Y,
    FACE_B_X,
    FACE_B_Y
};

enum EdgeNumbers
{
    NO_EDGE = 0,
    EDGE1,
    EDGE2,
    EDGE3,
    EDGE4
};

struct ClipVertex
{
    ClipVertex() { fp.value = 0; }
    Vec2 v;
    FeaturePair fp;
};

struct ZeroColliderKey;
class ZeroCollider;

//World
class ZeroColliderManager {
public:
    ZeroColliderManager() = default;
    ~ZeroColliderManager();
    void Init(Vec2 gravity, int iterations);

    void AddRigidBody2D(RigidBody2D* body);
    void Clear();

    void Step(float dt);
    void BroadPhase();
    void EndScene();

    static int Collide(Contact* contacts, RigidBody2D* bodyA, RigidBody2D* bodyB);

    static bool accumulateImpulses;
    static bool warmStarting;
    static bool positionCorrection;

private:
    inline static void Flip(FeaturePair& fp);
    static int ClipSegmentToLine(
        ClipVertex vOut[2], ClipVertex vIn[2],
        const Vec2& normal, float offset, char clipEdge);
    static void ComputeIncidentEdge(ClipVertex c[2],
        const Vec2& h, const Vec2& pos,
        const Mat22& Rot, const Vec2& normal);

    std::vector<RigidBody2D*> bodies;
    std::map<ZeroColliderKey, ZeroCollider> arbiters;
    Vec2 gravity;
    int iterations;
};

//
//class ZeroColliderManager {
//public:
//    void MountCollider(BoxCollider* colA, BoxCollider* colB);
//
//    std::vector<std::unordered_set<BoxCollider*>> GetColliderQueue();
//    void Update();
//    void LateUpdate();
//
//private:
//    std::vector<std::unordered_set<BoxCollider*>> colliderQueue{};
//};


#endif //ZERO_ENGINE_ZEROCOLLIDERMANAGER_H
