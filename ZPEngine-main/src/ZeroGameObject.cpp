//
// Created by EunwooSong on 2020-12-06.
//

#include "stdafx.h"
#include "ZeroGameObject.h"
#include "Transform.h"
#include "RectTransform.h"

void ZeroGameObject::Init() {
    ZeroEntity::Init();
    if (isUIObject)
        this->transform = (Transform*)AddComponent<RectTransform>();
    else
        this->transform = AddComponent<Transform>();
}
