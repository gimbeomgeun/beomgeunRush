//
// Created by EunwooSong on 2020-12-06.
//

#ifndef ZERO_ENGINE_ZEROENTITY_H
#define ZERO_ENGINE_ZEROENTITY_H

#include "ZeroDefine.h"
#include <vector>
#include <string>

class ZeroScene;
class ZeroComponent;

typedef class ZeroEntity {
public:
    ZeroEntity();
    ~ZeroEntity();

    virtual void Init() {};
    void EndScene();    // 삭제될 컴포넌트 삭제를 위함

    void SetEntityID(EntityID id) { if(m_id == -1) this->m_id = id; }
    void SetTag(std::string tag) { this->tag = tag; }
    void SetName(std::string name) { this->name = name; }
    std::string GetName() { return name; }
    std::string GetTag() { return tag; }
    bool GetIsDestroy() { return isDestroy; }
    bool GetIsActive() { return isActive; };
    void SetIsActive(bool isActive);

    ZeroScene* GetScene() { return m_Scene; }
    EntityID GetEntityID() { return m_id; };

    static void Destroy(ZeroEntity* iter) { iter->SetIsDestroy(true); }

protected:
    void SetIsDestroy(bool isDestroy);

    EntityID m_id;
    std::string name;
    std::string tag;
    ZeroScene* m_Scene;
    std::vector<ZeroComponent*> m_components;
    bool isDestroy;
    bool isActive;

    friend ZeroScene;
} Entity;


#endif //ZERO_ENGINE_ZEROENTITY_H
