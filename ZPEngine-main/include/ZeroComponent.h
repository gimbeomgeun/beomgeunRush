//
// Created by EunwooSong on 2020-12-06.
//

#ifndef ZERO_ENGINE_ZEROCOMPONENT_H
#define ZERO_ENGINE_ZEROCOMPONENT_H

#include "ZeroDefine.h"

class ZeroGameObject;
class ZeroEntity;
class BoxCollider;

// -= ZeroComponent =-
// - 컴포넌트를 생성할 때 필수적으로 필요한 컴포넌트
typedef class ZeroComponent {
public:
    ZeroComponent() : isDestroy(false), isActive(true), isStarted(false), m_Owner(nullptr) {};
    virtual ~ZeroComponent() { };
    virtual void Start() { isStarted = true; };
    virtual void Update() {}
    virtual void LateUpdate() {}
    virtual void Render() {}
    virtual void EndScene() {}

    virtual void FixedUpdate() {}

    void Destroy()                          { isDestroy = true; }
    static void Destroy(ZeroComponent* compo)   { compo->Destroy(); }
    bool GetIsDestroy() const               { return isDestroy; }

    virtual void SetActive(bool isActive)           { this->isActive = isActive; }
    bool GetActive() const                  { return isActive; }

    ZeroGameObject* GetOwner();
    void SetOwner(ZeroEntity* owner)            { m_Owner = owner; }

    bool GetIsStarted()                     { return isStarted; }

protected:
    ZeroEntity* m_Owner;
    bool isDestroy;
    bool isActive;
    bool isStarted;

    //Component Listeners
public:
    virtual void OnCollisionEnter(ZeroGameObject* coll) {};
    virtual void OnCollisionStay(ZeroGameObject* coll) {};
    virtual void OnCollisionExit(ZeroGameObject* coll) {};
    virtual void OnTriggerEnter(ZeroGameObject* coll) {};
    virtual void OnTriggerStay(ZeroGameObject* coll) {};
    virtual void OnTriggerExit(ZeroGameObject* coll) {};

    virtual void OnCursorHover() {};    // 커서가 객체 위에 있을 때
    virtual void OnCursorEnter() {};    // 커서가 객체 위에서 클릭을 시작했을 때
    virtual void OnCursorStay() {};     // 클릭한 객체에서 커서가 클릭을 유지하고 있을 때 (객체 속, 안 구분 없이...)
    virtual void OnCursorExit() {};     // 커서가 클릭을 종료했을 때
} Component;


#endif //ZERO_ENGINE_ZEROCOMPONENT_H
