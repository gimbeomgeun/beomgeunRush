//
// Created by joonl on 2020-12-25.
//

#include "stdafx.h"
#include "Button.h"
#include "ZeroSystem.h"
#include "ZeroGameObject.h"
#include "Sprite2DRenderer.h"
#include "UIRenderer.h"
#include "ZeroMath.h"

using namespace ZeroMath;

Button::~Button() {
}
void Button::Start() {
    Component::Start();

    boundaryRect.left *= defScale.x;
    boundaryRect.right *= defScale.x;
    boundaryRect.top *= defScale.y;
    boundaryRect.bottom *= defScale.y;
}
void Button::Update() {
    Component::Update();

    Vec2 currentCursorPos = ZERO_INPUT_MGR->GetCursorPos();

    if(boundaryRect.offset(GetOwner()->transform->GetLocalPos()).Contain(currentCursorPos)) {
        isOnCursor = true;
        isEndCursor = true;
    }
    else {
        isOnCursor = false;
        isEndCursor = false;
    }

    if(isOnCursor) {
        if(ZERO_INPUT_MGR->GetKeyState(INPUT_MOUSE_LBUTTON) == KeyState::UP)
            isOnClick = true;
        else
            isOnClick = false;
    }

    if(isOnClick) {
        for(auto iter : onClick) {
            iter();
        }
    }

    if(isEnableAnimation) {
        if(isOnCursor) {
            GetOwner()->transform->SetScale(
                    Lerp(GetOwner()->transform->GetScale(),
                         upScale,
                         upSpeed * ZERO_TIME_MGR->GetDeltaTime()));
        }
        else {
            GetOwner()->transform->SetScale(
                    Lerp(GetOwner()->transform->GetScale(),
                         defScale,
                         upSpeed * ZERO_TIME_MGR->GetDeltaTime()));

        }
    }
    else {
        GetOwner()->transform->SetScale(
                Lerp(GetOwner()->transform->GetScale(),
                     defScale,
                     upSpeed * ZERO_TIME_MGR->GetDeltaTime()));
    }
}
void Button::SetTextureImage(std::string path) {
    UIRenderer *renderer = GetOwner()->GetComponent<UIRenderer>();
    renderer->SetTexture(path);
    float width = renderer->GetWidth() / 2.0f;
    float height = renderer->GetHeight() / 2.0f;
    boundaryRect = Rect(-width, -height, width, height);
    //GetOwner()->transform->SetScaleCenter(Vec2(renderer->GetWidth() / 2, renderer->GetHeight() / 2));
    //GetOwner()->transform->SetRotationCenter(Vec2(renderer->GetWidth() / 2, renderer->GetHeight() / 2));
}

void Button::SetActive(bool active) {
    if(this->isActive != active) {
        isOnCursor = false;
        isOnClick = false;
        isEndCursor = false;

        GetOwner()->transform->SetScale(defScale);
    }

    ZeroComponent::SetActive(active);
}
