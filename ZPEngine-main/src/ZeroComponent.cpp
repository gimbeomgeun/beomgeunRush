//
// Created by EunwooSong on 2020-12-06.
//

#include "stdafx.h"
#include "ZeroEntity.h"
#include "ZeroGameObject.h"
#include "ZeroComponent.h"


ZeroGameObject *ZeroComponent::GetOwner() {
    return dynamic_cast<ZeroGameObject*>(m_Owner);
}
