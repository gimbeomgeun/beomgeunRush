//
// Created by EunwooSong on 2020-12-06.
//

#ifndef ZERO_ENGINE_ZEROSCENE_H
#define ZERO_ENGINE_ZEROSCENE_H


#include <list>
#include <map>
#include <vector>

#include "ZeroDefine.h"
#include "ZeroEntityIDManager.h"
#include "ZeroComponentManager.h"
#include "ZeroColliderManager.h"
#include "ZeroEventManager.h"
#include "ZeroEntity.h"
#include "ZeroRenderManager.h"

class ZeroGameObject;

typedef class ZeroScene {
public:
    ZeroScene();
    ~ZeroScene();

    //Hierarchy, Initialize gameObject
    //Ex, Player* player = new Player();
    virtual void Init();
    void Start();
    void Update();
    void LateUpdate();
    void Render();
    void EndScene();

    void FixedUpdate();

    //Entity 등록
    EntityID CreateEntity(ZeroEntity* iter);

    //Entity 삭제
    void DestroyEntity(EntityID _id);

    //컴포넌트 등록
    template <typename T>
    void RegisterComponent() {
        m_ComponentManager->RegisterComponent<T>();
    }

    //엔터티에 등록된 컴포넌트 삭제
    template <typename T>
    void DestroyComponent(EntityID _id) {
        m_ComponentManager->DestroyComponent<T>(_id);

        auto signature = m_EntityIDManager->GetSignature(_id);
        signature.set(m_ComponentManager->GetComponentID<T>(), false);
    }

    //엔터티에 컴포넌트 추가
    template <typename T>
    void AddComponent(ZeroEntity* _id, T* iter) {
        m_ComponentManager->AddComponent<T>(_id->GetEntityID(), iter);
        dynamic_cast<Component*>(iter)->SetOwner(_id);
        auto signature = m_EntityIDManager->GetSignature(_id->GetEntityID());
        signature.set(m_ComponentManager->GetComponentID<T>(), true);
        m_EntityIDManager->SetSignature(_id->GetEntityID(), signature);
    };

    //엔터티에 등록되어있는 컴포넌트를 불러옴
    template <typename T>
    T* GetComponent(EntityID _id) {
        return m_ComponentManager->GetComponent<T>(_id);
    }

    template <typename T>
    ComponentID GetComponentType() {
        return m_ComponentManager->GetComponentID<T>();
    }

    template <typename T>
    std::vector<T*> GetComponentArray() {
        return m_ComponentManager->GetTypeComponentList<T>();
    }

    ZeroGameObject* GetMainCamera() {return mainCamera;};
    std::vector<ZeroComponent*> FindEntityComponents(EntityID _id);
    ZeroGameObject* FindGameObject(std::string name);
    ZeroGameObject* FindGameObjectWithTag(std::string tag);


    void BindRigidBody(ZeroGameObject* body);
    ZeroRenderManager*      GetRenderManager();
    ZeroColliderManager*    GetColliderManager()    { return m_ColliderManager.get(); };
    ZeroEventManager*       GetEventManager()       { return m_EventManager.get(); }
    std::map<EntityID, ZeroEntity*> m_EntityList;

    //For Test
    bool enablePhysics = true;

protected:
    //- = System Managers = -
    std::unique_ptr<ZeroComponentManager>       m_ComponentManager;
    std::unique_ptr<ZeroEntityIDManager>        m_EntityIDManager;
    std::unique_ptr<ZeroColliderManager>        m_ColliderManager;
    std::unique_ptr<ZeroEventManager>           m_EventManager;

    ZeroGameObject* mainCamera;

    int fixedRate = 60;
    float fixedTimer;
} Scene;

#endif //ZERO_ENGINE_ZEROSCENE_H
