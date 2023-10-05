//
// Created by EunwooSong on 2020-12-07.
//

#include "stdafx.h"
#include "Transform.h"
#include "ZeroGameObject.h"
#include "ZeroMath.h"

void Transform::Start() {
    ZeroComponent::Start();

    if (!parent)
        UpdateTransformation();
}

void Transform::Update() {
    ZeroComponent::Update();
}

void Transform::LateUpdate() {
    ZeroComponent::LateUpdate();
}

void Transform::Render() {
    ZeroComponent::Render();

    if (!parent)
        UpdateTransformation();
}


void Transform::EndScene() {
    ZeroComponent::EndScene();

    if(parent)
        if (parent->GetOwner()->GetIsDestroy())
            GameObject::Destroy(GetOwner());
}

void Transform::UpdateTransformation() {

    mat = ZeroMath::MatrixTransformation(scaleCenter, scale, localRotation, localPos);

    worldPos = localPos;
    worldRotation = localRotation;

    if (parent) {
        mat *= parent->mat;
        worldPos += parent->GetWorldPos();
        worldRotation += parent->GetWorldRotation();
    }

    if (children.size() > 0)
        for (auto& child : children)
            child->UpdateTransformation();
}

void Transform::AddChild(Transform *iter) {
    iter->SetParent(this);
    children.push_back(iter);
}
void Transform::PopChild(Transform *iter) {

    children.erase(remove_if(children.begin(), children.end(), [&](auto i) {return i == iter; }), children.end());
}