//
// Created by EunwooSong on 2020-12-06.
//

#ifndef ZERO_ENGINE_ZEROGAMEOBJECT_H
#define ZERO_ENGINE_ZEROGAMEOBJECT_H


#include "ZeroEntity.h"
#include "ZeroComponent.h"
#include "ZeroScene.h"

class Transform;

typedef class ZeroGameObject :
        public ZeroEntity
{
public:
    ZeroGameObject(bool isUIObject = false) : transform(nullptr), isUIObject(isUIObject){ Init(); }
    ~ZeroGameObject() {}

    virtual void Init() override;

    template<typename T>
    T* AddComponent() {
        T* compo = new T();
        m_Scene->AddComponent<T>(this, compo);
        m_components.push_back(dynamic_cast<Component*>(compo));

        return compo;
    }

    template<typename T>
    T* GetComponent() {
        return m_Scene->GetComponent<T>(GetEntityID());
    }

    template <typename T>
    void DestroyComponent() {
        m_Scene->DestroyComponent<T>(GetEntityID());
    }

    Transform* transform;

private:
    bool isUIObject;
} GameObject;



#endif //ZERO_ENGINE_ZEROGAMEOBJECT_H
