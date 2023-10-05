//
// Created by EunwooSong on 2020-12-06.
//

#include "stdafx.h"
#include "ZeroEntityIDManager.h"

ZeroEntityIDManager::ZeroEntityIDManager() {
    for (EntityID entity = 0; entity < MAX_ENTITIES; ++entity)
    {
        m_AvailableEntityIDs.push(entity);
    }
}

ZeroEntityIDManager::~ZeroEntityIDManager()
{
    while (!m_AvailableEntityIDs.empty())
        m_AvailableEntityIDs.pop();

    m_Signatures.clear();
}

EntityID ZeroEntityIDManager::CreateEntityID() {
    assert(m_LivingEntityIDCount < MAX_ENTITIES && "Entity out of range.");

    EntityID id = m_AvailableEntityIDs.front();
    m_AvailableEntityIDs.pop();
    ++m_LivingEntityIDCount;

    if (m_Signatures.begin() + id == m_Signatures.end()) {
        Signature signature;
        m_Signatures.push_back(signature);
    }
    else m_Signatures[id].reset();
    return id;
}

void ZeroEntityIDManager::DestroyEntityID(EntityID _id) {
    assert(_id < MAX_ENTITIES && "Entity out of range.");
    m_Signatures[_id].reset();

    m_AvailableEntityIDs.push(_id);
    --m_LivingEntityIDCount;
}

void ZeroEntityIDManager::SetSignature(EntityID _id, Signature signature) {
    assert(_id < MAX_ENTITIES && "Entity out of range.");

    m_Signatures[_id] = signature;
}

Signature ZeroEntityIDManager::GetSignature(EntityID _id) {
    assert(_id < MAX_ENTITIES && "Entity out of range.");

    return m_Signatures[_id];
}