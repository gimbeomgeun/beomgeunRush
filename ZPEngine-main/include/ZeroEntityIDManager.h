//
// Created by EunwooSong on 2020-12-06.
//

#ifndef ZERO_ENGINE_ZEROENTITYIDMANAGER_H
#define ZERO_ENGINE_ZEROENTITYIDMANAGER_H

#include "ZeroDefine.h"
#include <array>
#include <cassert>
#include <queue>

class ZeroEntityIDManager {
public:
    ZeroEntityIDManager();
    ~ZeroEntityIDManager();

    EntityID CreateEntityID();
    void DestroyEntityID(EntityID _id);

    void SetSignature(EntityID _id, Signature signature);

    Signature GetSignature(EntityID _id);

private:
    std::queue<EntityID> m_AvailableEntityIDs{};
    std::vector<Signature> m_Signatures{};
    uint32_t m_LivingEntityIDCount{};
};


#endif //ZERO_ENGINE_ZEROENTITYIDMANAGER_H
