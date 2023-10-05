#include "..\include\ZeroScene.h"
//
// Created by EunwooSong on 2020-12-06.
//

#include "stdafx.h"
#include "ZeroLogger.h"
#include "ZeroScene.h"
#include "ZeroEntity.h"
#include "ZeroGameObject.h"
#include "Sprite2DRenderer.h"
#include "Camera2D.h"
#include "RigidBody2D.h"
#include "UIRenderer.h"
#include "Button.h"
#include "AnimationController.h"
#include "UITextRenderer.h"
#include "AudioPlayer.h"
#include "ZeroTimeMgr.h"
#include "ZeroSystem.h"

ZeroScene::ZeroScene() : mainCamera(nullptr), fixedTimer(0), fixedRate(60) {

}

ZeroScene::~ZeroScene() {
    // Reset Entities and Component . . .
    for (auto iter : m_EntityList) {
        SAFE_DELETE(iter.second);
    }
    m_ComponentManager->Release();

    // Reset Managers . . .
    m_ComponentManager.reset();
    m_ColliderManager.reset();
    m_EntityIDManager.reset();
    m_EventManager.reset();
}

void ZeroScene::Init() {
    m_ComponentManager = std::make_unique<ZeroComponentManager>();
    m_ColliderManager = std::make_unique<ZeroColliderManager>();
    m_EntityIDManager = std::make_unique<ZeroEntityIDManager>();
    m_EventManager = std::make_unique<ZeroEventManager>();

    //For Collider Test
    m_ColliderManager->Init(Vec2(0.0f, -10.0f), 10);

    //Register Component(Transform, Sprite2DRenderer, Camera2D etc . . .)
    RegisterComponent<Transform>();
    RegisterComponent<Sprite2DRenderer>();
    RegisterComponent<Camera2D>();
    RegisterComponent<RigidBody2D>();
    RegisterComponent<UIRenderer>();
    RegisterComponent<Button>();
    RegisterComponent<AnimationController>();
    RegisterComponent<UITextRenderer>();
    RegisterComponent<AudioPlayer>();

    mainCamera = new GameObject();
    mainCamera->AddComponent<Camera2D>();
    mainCamera->SetTag("MainCamera");

    fixedTimer = 0.0f;
}

void ZeroScene::Start() {
    m_ComponentManager->Start();
}

void ZeroScene::Update() {
    m_ComponentManager->Update();
    m_EventManager->Update();
}

void ZeroScene::LateUpdate() {
    //Check Colliders
    
    m_ComponentManager->LateUpdate();
}

void ZeroScene::Render() {
    m_ComponentManager->Render();
}

void ZeroScene::EndScene() {
    for (auto iter : m_EntityList) {
        iter.second->EndScene();
    }


    m_ColliderManager->EndScene();
    m_ComponentManager->EndScene();
    m_EventManager->EndScene();

    for (auto iter = m_EntityList.begin(); iter != m_EntityList.end(); ++iter) {
        if (iter->second->GetIsDestroy()) {
            auto tmp = iter++;
            DestroyEntity(tmp->first);
            if (iter != m_EntityList.begin())
                iter--;
        }
    }
}

void ZeroScene::FixedUpdate()
{
    fixedTimer += ZERO_TIME_MGR->GetDeltaTimeF();

    if (fixedTimer >= 1.0f / (float)fixedRate) {
        if (enablePhysics)
            m_ColliderManager->Step(1.0f / (float)fixedRate);

        m_ComponentManager->FixedUpdate();

        fixedTimer = 0.0f;
    }
}

EntityID ZeroScene::CreateEntity(ZeroEntity *iter) {

    EntityID tmp = m_EntityIDManager->CreateEntityID();
    m_EntityList.insert(std::pair<EntityID, Entity*>(tmp, iter));
    iter->SetEntityID(tmp);
    iter->Init();
    return tmp;
}

void ZeroScene::DestroyEntity(EntityID _id) {
    m_EntityIDManager->DestroyEntityID(_id);
    m_ComponentManager->EntityDestroyed(_id);

    SAFE_DELETE(m_EntityList[_id]);
    m_EntityList.erase(_id);
}

std::vector<ZeroComponent *> ZeroScene::FindEntityComponents(EntityID _id) {
    return m_EntityList[_id]->m_components;
}

ZeroGameObject *ZeroScene::FindGameObject(std::string name) {
    for(auto iter : m_EntityList) {
        if(iter.second->GetName().compare(name) == 0)
            return dynamic_cast<ZeroGameObject*>(iter.second);
    }

    ZERO_LOGGER::Error("Failed to find game object name : %s ", name.c_str());
    return nullptr;
}

ZeroGameObject *ZeroScene::FindGameObjectWithTag(std::string tag) {
    for(auto iter : m_EntityList) {
        if(iter.second->GetTag().compare(tag) == 0)
            return dynamic_cast<ZeroGameObject*>(iter.second);
    }

    ZERO_LOGGER::Error("Failed to find game object name : %s ", tag.c_str());
    return nullptr;
}

void ZeroScene::BindRigidBody(ZeroGameObject* obj) {
    m_ColliderManager.get()->AddRigidBody2D(obj->GetComponent<RigidBody2D>());
}

ZeroRenderManager* ZeroScene::GetRenderManager()
{
    return ZERO_RENDERER;
}