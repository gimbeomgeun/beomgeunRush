//
// Created by EunwooSong on 2020-12-12.
//

#include "stdafx.h"
#include "ZeroColliderManager.h"
#include "ZeroTimeMgr.h"
#include "ZeroGameObject.h"
#include "RigidBody2D.h"
#include "Transform.h"
#include "ZeroColliderGroup.h"

using namespace ZeroMath;

typedef std::map<ZeroColliderKey, ZeroCollider>::iterator ArbIter;
typedef std::pair<ZeroColliderKey, ZeroCollider> ArbPair;

bool ZeroColliderManager::accumulateImpulses = true;
bool ZeroColliderManager::warmStarting = true;
bool ZeroColliderManager::positionCorrection = true;

ZeroColliderManager::~ZeroColliderManager()
{
    Clear();
}

void ZeroColliderManager::Flip(FeaturePair &fp) {
    Swap(fp.e.inEdge1, fp.e.inEdge2);
    Swap(fp.e.outEdge1, fp.e.outEdge2);
}

int ZeroColliderManager::ClipSegmentToLine(ClipVertex *vOut, ClipVertex *vIn, const Vec2 &normal, float offset,
                                           char clipEdge) {
    // Start with no output points
    int numOut = 0;

    // Calculate the distance of end points to the line
    float distance0 = Dot(normal, vIn[0].v) - offset;
    float distance1 = Dot(normal, vIn[1].v) - offset;

    // If the points are behind the plane
    if (distance0 <= 0.0f) vOut[numOut++] = vIn[0];
    if (distance1 <= 0.0f) vOut[numOut++] = vIn[1];

    // If the points are on different sides of the plane
    if (distance0 * distance1 < 0.0f)
    {
        // Find intersection point of edge and plane
        float interp = distance0 / (distance0 - distance1);
        vOut[numOut].v = vIn[0].v + interp * (vIn[1].v - vIn[0].v);
        if (distance0 > 0.0f)
        {
            vOut[numOut].fp = vIn[0].fp;
            vOut[numOut].fp.e.inEdge1 = clipEdge;
            vOut[numOut].fp.e.inEdge2 = NO_EDGE;
        }
        else
        {
            vOut[numOut].fp = vIn[1].fp;
            vOut[numOut].fp.e.outEdge1 = clipEdge;
            vOut[numOut].fp.e.outEdge2 = NO_EDGE;
        }
        ++numOut;
    }

    return numOut;
}

void ZeroColliderManager::ComputeIncidentEdge(ClipVertex *c, const Vec2 &h, const Vec2 &pos, const Mat22 &Rot,
                                              const Vec2 &normal) {
    // The normal is from the reference box. Convert it
    // to the incident boxe's frame and flip sign.
    Mat22 RotT = Rot.Transpose();
    Vec2 n = -(RotT * normal);
    Vec2 nAbs = Abs(n);

    if (nAbs.x > nAbs.y)
    {
        if (Sign(n.x) > 0.0f)
        {
            c[0].v.Set(h.x, -h.y);
            c[0].fp.e.inEdge2 = EDGE3;
            c[0].fp.e.outEdge2 = EDGE4;

            c[1].v.Set(h.x, h.y);
            c[1].fp.e.inEdge2 = EDGE4;
            c[1].fp.e.outEdge2 = EDGE1;
        }
        else
        {
            c[0].v.Set(-h.x, h.y);
            c[0].fp.e.inEdge2 = EDGE1;
            c[0].fp.e.outEdge2 = EDGE2;

            c[1].v.Set(-h.x, -h.y);
            c[1].fp.e.inEdge2 = EDGE2;
            c[1].fp.e.outEdge2 = EDGE3;
        }
    }
    else
    {
        if (Sign(n.y) > 0.0f)
        {
            c[0].v.Set(h.x, h.y);
            c[0].fp.e.inEdge2 = EDGE4;
            c[0].fp.e.outEdge2 = EDGE1;

            c[1].v.Set(-h.x, h.y);
            c[1].fp.e.inEdge2 = EDGE1;
            c[1].fp.e.outEdge2 = EDGE2;
        }
        else
        {
            c[0].v.Set(-h.x, -h.y);
            c[0].fp.e.inEdge2 = EDGE2;
            c[0].fp.e.outEdge2 = EDGE3;

            c[1].v.Set(h.x, -h.y);
            c[1].fp.e.inEdge2 = EDGE3;
            c[1].fp.e.outEdge2 = EDGE4;
        }
    }

    c[0].v = pos + Rot * c[0].v;
    c[1].v = pos + Rot * c[1].v;
}

int ZeroColliderManager::Collide(Contact *contacts, RigidBody2D *bodyA, RigidBody2D *bodyB) {
    // Setup
    Vec2 hA = 0.5f * bodyA->width;
    Vec2 hB = 0.5f * bodyB->width;

    Vec2 posA = bodyA->GetOwner()->transform->GetWorldPos();
    Vec2 posB = bodyB->GetOwner()->transform->GetWorldPos();

    Mat22 RotA(bodyA->GetOwner()->transform->GetWorldRotation()), RotB(bodyB->GetOwner()->transform->GetWorldRotation());

    Mat22 RotAT = RotA.Transpose();
    Mat22 RotBT = RotB.Transpose();

    Vec2 dp = posB - posA;
    Vec2 dA = RotAT * dp;
    Vec2 dB = RotBT * dp;

    Mat22 C = RotAT * RotB;
    Mat22 absC = Abs(C);
    Mat22 absCT = absC.Transpose();

    //SAT 검사 진행
    // Box A의 공간에서의 검사
    Vec2 faceA = Abs(dA) - hA - absC * hB;
    if (faceA.x > 0.0f || faceA.y > 0.0f)
        return 0;

    // Box B의 공간에서의 검사
    Vec2 faceB = Abs(dB) - absCT * hA - hB;
    if (faceB.x > 0.0f || faceB.y > 0.0f)
        return 0;

    // Find best axis
    Axis axis;
    float separation;
    Vec2 normal;

    // Box A를 기준으로 분리할 축 찾기
    axis = FACE_A_X;
    separation = faceA.x;
    normal = dA.x > 0.0f ? RotA.col1 : -RotA.col1;

    const float relativeTol = 0.95f;
    const float absoluteTol = 0.01f;

    if (faceA.y > relativeTol * separation + absoluteTol * hA.y)
    {
        axis = FACE_A_Y;
        separation = faceA.y;
        normal = dA.y > 0.0f ? RotA.col2 : -RotA.col2;
    }

    // Box B를 기준으로 분리할 축 찾기
    if (faceB.x > relativeTol * separation + absoluteTol * hB.x)
    {
        axis = FACE_B_X;
        separation = faceB.x;
        normal = dB.x > 0.0f ? RotB.col1 : -RotB.col1;
    }

    if (faceB.y > relativeTol * separation + absoluteTol * hB.y)
    {
        axis = FACE_B_Y;
        separation = faceB.y;
        normal = dB.y > 0.0f ? RotB.col2 : -RotB.col2;
    }

    // Setup clipping plane data based on the separating axis
    Vec2 frontNormal, sideNormal;
    ClipVertex incidentEdge[2];
    float front, negSide, posSide;
    char negEdge, posEdge;

    // Compute the clipping lines and the line segment to be clipped.
    switch (axis)
    {
        case FACE_A_X:
        {
            frontNormal = normal;
            front = Dot(posA, frontNormal) + hA.x;
            sideNormal = RotA.col2;
            float side = Dot(posA, sideNormal);
            negSide = -side + hA.y;
            posSide =  side + hA.y;
            negEdge = EDGE3;
            posEdge = EDGE1;
            ComputeIncidentEdge(incidentEdge, hB, posB, RotB, frontNormal);
        }
            break;

        case FACE_A_Y:
        {
            frontNormal = normal;
            front = Dot(posA, frontNormal) + hA.y;
            sideNormal = RotA.col1;
            float side = Dot(posA, sideNormal);
            negSide = -side + hA.x;
            posSide =  side + hA.x;
            negEdge = EDGE2;
            posEdge = EDGE4;
            ComputeIncidentEdge(incidentEdge, hB, posB, RotB, frontNormal);
        }
            break;

        case FACE_B_X:
        {
            frontNormal = -normal;
            front = Dot(posB, frontNormal) + hB.x;
            sideNormal = RotB.col2;
            float side = Dot(posB, sideNormal);
            negSide = -side + hB.y;
            posSide =  side + hB.y;
            negEdge = EDGE3;
            posEdge = EDGE1;
            ComputeIncidentEdge(incidentEdge, hA, posA, RotA, frontNormal);
        }
            break;

        case FACE_B_Y:
        {
            frontNormal = -normal;
            front = Dot(posB, frontNormal) + hB.y;
            sideNormal = RotB.col1;
            float side = Dot(posB, sideNormal);
            negSide = -side + hB.x;
            posSide =  side + hB.x;
            negEdge = EDGE2;
            posEdge = EDGE4;
            ComputeIncidentEdge(incidentEdge, hA, posA, RotA, frontNormal);
        }
            break;
    }

    // clip other face with 5 box planes (1 face plane, 4 edge planes)

    ClipVertex clipPoints1[2];
    ClipVertex clipPoints2[2];
    int np;

    // Clip to box side 1
    np = ClipSegmentToLine(clipPoints1, incidentEdge, -sideNormal, negSide, negEdge);

    if (np < 2)
        return 0;

    // Clip to negative box side 1
    np = ClipSegmentToLine(clipPoints2, clipPoints1,  sideNormal, posSide, posEdge);

    if (np < 2)
        return 0;

    // Now clipPoints2 contains the clipping points.
    // Due to roundoff, it is possible that clipping removes all points.

    int numContacts = 0;
    for (int i = 0; i < 2; ++i)
    {
        float separation = Dot(frontNormal, clipPoints2[i].v) - front;

        if (separation <= 0)
        {
            contacts[numContacts].separation = separation;
            contacts[numContacts].normal = normal;
            // slide contact point onto reference face (easy to cull)
            contacts[numContacts].position = clipPoints2[i].v - separation * frontNormal;
            contacts[numContacts].feature = clipPoints2[i].fp;
            if (axis == FACE_B_X || axis == FACE_B_Y)
                Flip(contacts[numContacts].feature);
            ++numContacts;
        }
    }

    auto aIter = bodyA->GetOwner()->GetScene()->FindEntityComponents(bodyA->GetOwner()->GetEntityID());
    auto bIter = bodyB->GetOwner()->GetScene()->FindEntityComponents(bodyB->GetOwner()->GetEntityID());

    //A의 함수 호출
    for(auto iter : aIter)
        iter->OnCollisionStay(bodyB->GetOwner());

    //B의 함수 호출
    for(auto iter : bIter)
        iter->OnCollisionStay(bodyA->GetOwner());

    return numContacts;
}

void ZeroColliderManager::Init(Vec2 gravity, int iterations) {
    this->gravity = gravity;
    this->iterations = iterations;
}

void ZeroColliderManager::AddRigidBody2D(RigidBody2D *body) {
    if(body) bodies.push_back(body);
}

void ZeroColliderManager::Clear() {
    bodies.clear();
    arbiters.clear();
}

void ZeroColliderManager::BroadPhase() {
    // O(n^2) broad-phase
    for (int i = 0; i < (int)bodies.size(); ++i)
    {
        RigidBody2D* bi = bodies[i];

        for (int j = i + 1; j < (int)bodies.size(); ++j)
        {
            RigidBody2D* bj = bodies[j];

            if (bi->invMass == 0.0f && bj->invMass == 0.0f)
                continue;

            ZeroCollider newArb(bi, bj);
            ZeroColliderKey key(bi, bj);

            if (newArb.numContacts > 0)
            {
                ArbIter iter = arbiters.find(key);
                if (iter == arbiters.end())
                {
                    arbiters.insert(ArbPair(key, newArb));
                }
                else
                {
                    iter->second.Update(newArb.contacts, newArb.numContacts);
                }
            }
            else
            {
                arbiters.erase(key);
            }
        }
    }
}

//Update Float dt
void ZeroColliderManager::Step(float dt) {
    float inv_dt = dt > 0.0f? 1.0f / dt : 0.0f;

    // Determine overlapping bodies and update contact points.
    BroadPhase();

    // Integrate forces.
    for (int i = 0; i < (int)bodies.size(); ++i)
    {
        RigidBody2D* b = bodies[i];

        if (b->invMass == 0.0f)
            continue;

        b->velocity += dt * (gravity + b->invMass * b->force);
        b->angularVelocity += dt * b->invI * b->torque;
    }

    // Perform pre-steps.
    for (ArbIter arb = arbiters.begin(); arb != arbiters.end(); ++arb)
    {
        arb->second.PreStep(inv_dt);
    }
    // Perform iterations
    for (int i = 0; i < iterations; ++i)
    {
        for (ArbIter arb = arbiters.begin(); arb != arbiters.end(); ++arb)
        {
            arb->second.ApplyImpulse();
        }
    }

    // Integrate Velocities
    for (int i = 0; i < (int)bodies.size(); ++i)
    {
        RigidBody2D* b = bodies[i];

        b->GetOwner()->transform->Translate(dt * b->velocity);
        if(!b->freezeRotation)
            b->GetOwner()->transform->Rotate(dt * b->angularVelocity);
        else
            b->GetOwner()->transform->SetRotationRadians(b->GetOwner()->transform->GetWorldRotation());

        b->force.Set(0.0f, 0.0f);
        b->torque = 0.0f;

        //ZERO_LOGGER::Info("Body%d Velocity : %f %f", i, b->velocity.x, b->velocity.y);
    }
}

void ZeroColliderManager::EndScene() {
    bodies.erase(remove_if(bodies.begin(), bodies.end(),
                              [&](RigidBody2D* rigid) {
        return rigid->GetOwner()->GetIsDestroy();
    }),bodies.end());

//    arbiters.erase(remove_if(arbiters.begin(), arbiters.end(),
//                             [=](std::pair<const ZeroColliderKey,ZeroCollider>& key) {
//        return key.first.body1->GetOwner()->GetIsDestroy() ||  key.first.body2->GetOwner()->GetIsDestroy();
//    }), arbiters.end());

    for(auto iter = arbiters.begin(); iter != arbiters.end();) {
        if(iter->first.body1->GetOwner()->GetIsDestroy() || iter->first.body2->GetOwner()->GetIsDestroy()) {
            iter = arbiters.erase(iter);
        }
        else
            ++iter;
    }
}
