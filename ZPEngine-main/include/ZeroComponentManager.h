//
// Created by EunwooSong on 2020-12-06.
//

#ifndef ZERO_ENGINE_ZEROCOMPONENTMANAGER_H
#define ZERO_ENGINE_ZEROCOMPONENTMANAGER_H


#include "ZeroComponentArray.h"
#include "ZeroDefine.h"

class ZeroComponent;

class ZeroComponentManager {
public:
    ZeroComponentManager() = default;
    ~ZeroComponentManager();

    //컴포넌트 등록(새로운 ComponentArray 생성)
    //컴포넌트 종류별로 ComponentArray가 생성됨
    //Transform -> ComponentArray<Transform> / Sprite2DRenderer -> ComponentArray<Sprite2DRenderer>
    template<typename T>
    void RegisterComponent() {
        const char* typeName = typeid(T).name();

        assert(m_ComponentIDs.find(typeName)
            == m_ComponentIDs.end() && "Registering Component type more than once.");

        m_ComponentIDs.insert(std::pair<const char*, ComponentID>(typeName, m_nextComponentID));
        m_ComponentArrays.insert(
            std::pair<const char*,
            std::shared_ptr<IComponentArray>>(typeName,
                std::make_shared<ComponentArray<T>>()));
        ++m_nextComponentID;
    }

    //등록된 컴포넌트의 ID(인덱스 번호..)를 찾음
    template<typename T>
    ComponentID GetComponentID() {
        const char* typeName = typeid(T).name();
        return m_ComponentIDs[typeName];
    }

    //EntityID와 ComponentID를 맵핑함
    template<typename T>
    void AddComponent(EntityID _id, T* component) {
        GetComponentArray<T>()->AddComponent(_id, component);
    }

    //EntityID에 들어가있는 Component를 가져옴
    template<typename T>
    T* GetComponent(EntityID _id) {
        return GetComponentArray<T>()->GetComponent(_id);
    }

    //EntityID에 들어가 있는 Component를 삭제함
    template<typename T>
    void DestroyComponent(EntityID _id) {
        GetComponentArray<T>()->DestroyComponent(_id);
    }

    //Entity가 삭제 되었을 떄, 등록된 모든 컴포넌트를 삭제함
    void EntityDestroyed(EntityID _id) {
        for (auto const& pair : m_ComponentArrays)
        {
            auto const& component = pair.second;

            component->EntityDestroyed(_id);
        }
    }

    void Start();
    void Update();
    void LateUpdate();
    void Render();
    void EndScene();
    void FixedUpdate();

    void Release();

    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray()
    {
        const char* typeName = typeid(T).name();

        if(m_ComponentArrays.find(typeName) == m_ComponentArrays.end()) {
            ZERO_LOGGER::Info("RegisterComponent . . . %s", typeName);
            RegisterComponent<T>();
        }

        return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
    }

    template<typename T>
    std::vector<T*> GetTypeComponentList() {
        std::vector<T*> iter;
        const char* typeName = typeid(T).name();

        for (auto j : m_ComponentArrays[typeName]->GetComponentArray())
            if (dynamic_cast<T*>(j) != nullptr)
                iter.push_back(dynamic_cast<T*>(j));

        return iter;
    }

private:
    std::unordered_map<const char*, ComponentID>m_ComponentIDs{};
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays{};

    ComponentID m_nextComponentID;
};

#endif //ZERO_ENGINE_ZEROCOMPONENTMANAGER_H
