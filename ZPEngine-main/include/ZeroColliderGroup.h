//
// Created by EunwooSong on 2020-12-22.
//

#ifndef ZERO_ENGINE_ZEROCOLLIDERGROUP_H
#define ZERO_ENGINE_ZEROCOLLIDERGROUP_H

#include "ZeroDefine.h"

class RigidBody2D;

//Collider Key
struct ZeroColliderKey
{
    ZeroColliderKey(RigidBody2D* b1, RigidBody2D* b2)
    {
        if (b1 < b2)
        {
            body1 = b1; body2 = b2;
        }
        else
        {
            body1 = b2; body2 = b1;
        }
    }

    RigidBody2D* body1;
    RigidBody2D* body2;
};

class ZeroCollider
{
public:
    ZeroCollider(RigidBody2D* b1, RigidBody2D* b2);

    void Update(Contact* contacts, int numContacts);

    void PreStep(float inv_dt);
    void ApplyImpulse();

    Contact contacts[MAX_POINTS];
    int numContacts;

    RigidBody2D* body1;
    RigidBody2D* body2;

    // Combined friction
    float friction;
};

inline bool operator < (const ZeroColliderKey& a1, const ZeroColliderKey& a2)
{
    if (a1.body1 < a2.body1)
        return true;

    if (a1.body1 == a2.body1 && a1.body2 < a2.body2)
        return true;

    return false;
}

#endif //ZERO_ENGINE_ZEROCOLLIDERGROUP_H
