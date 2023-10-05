//
// Created by EunwooSong on 2020-12-06.
//

#ifndef ZERO_ENGINE_ZEROCOMPONENTARRAY_H
#define ZERO_ENGINE_ZEROCOMPONENTARRAY_H

#include "stdafx.h"
#include "ZeroDefine.h"
#include "ZeroComponent.h"
#include <iostream>
#include <array>
#include <cassert>
#include <unordered_map>

class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(EntityID _id) = 0;
    virtual std::vector<ZeroComponent*> GetComponentArray() = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
public:
    ~ComponentArray() {
        for (auto iter : m_ComponentArray)
            SAFE_DELETE(iter);

        m_ComponentArray.clear();
        m_EntityToIndexMap.clear();
        m_IndexToEntityMap.clear();
    }

    //EntityID와 컴포넌트의 맵핑을 진행합니다.
    void AddComponent(EntityID _id, T* component) {
        assert(m_EntityToIndexMap.find(_id) == m_EntityToIndexMap.end() && "Component added to same entity more than once.");

        //맵핑 진행 : id -> ComponentIndex, ComponentIndex -> id
        size_t newIndex = m_Size;
        m_EntityToIndexMap[_id] = newIndex;
        m_IndexToEntityMap[newIndex] = _id;
        m_ComponentArray.push_back(component);

        m_Size++;
    }

    //등록된 컴포넌트의 삭제를 진행합니다.
    void DestroyComponent(EntityID _id) {
        assert(m_EntityToIndexMap.find(_id) != m_EntityToIndexMap.end() && "Removing non-existent component");

        size_t indexOfRemovedEntity = m_EntityToIndexMap[_id];
        size_t indexOfLastElement = m_Size - 1;
        SAFE_DELETE(
                m_ComponentArray[indexOfRemovedEntity]);
        m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];
        m_ComponentArray.erase(m_ComponentArray.begin() + indexOfLastElement);

        EntityID entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
        m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
        m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

        m_EntityToIndexMap.erase(_id);
        m_IndexToEntityMap.erase(indexOfLastElement);

        --m_Size;
    }

    //EntityID에 등록된 컴포넌트를 반환합니다.
    T* GetComponent(EntityID _id) {
        //assert(m_EntityToIndexMap.find(_id) != m_EntityToIndexMap.end() && "Retrieving non-existent component");
        if (m_EntityToIndexMap.find(_id) == m_EntityToIndexMap.end()) {
            std::cout << "Retrieving non-existent component" << std::endl;
            return nullptr;
        }

        //맵핑된 컴포넌트 반환
        return m_ComponentArray[m_EntityToIndexMap[_id]];
    }

    void EntityDestroyed(EntityID _id) override {
        if (m_EntityToIndexMap.find(_id) != m_EntityToIndexMap.end()) {
            // Remove the entity's component if it existed
            DestroyComponent(_id);
        }
    }

    std::vector<ZeroComponent*> GetComponentArray() override {
        std::vector<ZeroComponent*> m_compoArray{};
        for (auto iter : m_ComponentArray)
            m_compoArray.push_back(dynamic_cast<Component*>(iter));

        return m_compoArray;
    }

private:
    std::vector<T*> m_ComponentArray;
    std::unordered_map<EntityID, size_t> m_EntityToIndexMap;
    std::unordered_map<size_t, EntityID> m_IndexToEntityMap;

    // Total size of valid entries in the array.
    size_t m_Size;
};


#endif //ZERO_ENGINE_ZEROCOMPONENTARRAY_H
