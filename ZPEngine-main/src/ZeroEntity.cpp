//
// Created by EunwooSong on 2020-12-06.
//

#include "stdafx.h"
#include "ZeroEntity.h"
#include "ZeroSystem.h"
#include "ZeroScene.h"
#include "ZeroComponent.h"

ZeroEntity::ZeroEntity() : m_id(-1), m_Scene(nullptr), isDestroy(false), isActive(true)
{
    m_Scene = ZERO_SCENE_MGR->GetCurrentScene();
    m_Scene->CreateEntity(this);
}

ZeroEntity::~ZeroEntity()
{
    name.clear();
    tag.clear();
    m_components.clear();
}

void ZeroEntity::EndScene()
{
    // 삭제될 컴포넌트 리스트에서 삭제...
    m_components.erase(std::remove_if(m_components.begin(), m_components.end(),
        [](ZeroComponent* iter) {
            return iter->GetIsDestroy();
        }), m_components.end());
}


void ZeroEntity::SetIsActive(bool isActive) {
    this->isActive = isActive;

    for (auto iter : m_components) {
        iter->SetActive(isActive);
    }
}

void ZeroEntity::SetIsDestroy(bool isDestroy) {
    this->isDestroy = isDestroy;
}