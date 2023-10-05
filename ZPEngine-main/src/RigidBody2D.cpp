//
// Created by EunwooSong on 2020-12-12.
//

#include "stdafx.h"
#include "RigidBody2D.h"
#include "ZeroTimeMgr.h"
#include "ZeroGameObject.h"
#include "Transform.h"
#include "ZeroSceneMgr.h"
#include "ZeroSystem.h"
#include "Camera2D.h"

// Back Up
//void RigidBody2D::LateUpdate() {
//    auto deltaTime = ZERO_TIME_MGR->GetDeltaTime();
//
//    if (!this->strict) {
//        Vec2 vel{ 0.0, 0.0 };
//
//        vel.y += deltaTime * GetGravity();
//
//        AddVelocity(vel);
//
//        GetOwner()->transform->Translate(
//                Vec2(deltaTime * GetVelocity().x, deltaTime * GetVelocity().y));
//    }
//    else {
//        SetVelocity(Vec2(0.0, 0.0));
//    }
//}
RigidBody2D::RigidBody2D() :
    velocity(0,0),
    angularVelocity(0),
    force(0, 0),
    torque(0),
    mass(FLT_MAX),
    friction(0.2f),
    width(1.0f, 1.0f),
    invMass(0),
    I(FLT_MAX),
    invI(0),
    debugColor(1.0f,1.0f,1.0f,1.0f),
    renderTrigger(false){
}

void RigidBody2D::Start() {
    ZeroComponent::Start();

    transform = GetOwner()->transform;

#if defined(_DEBUG) | defined(DEBUG)
    this->shader = ZERO_RESOURCES_MGR->LoadShaders("Box Shader", "shaders/BoxShader/BoxVertexShader.vert", "shaders/BoxShader/BoxFragmentShader.frag", "shaders/BoxShader/BoxGeometryShader.geom");
    //this->texture = ZERO_RESOURCES_MGR->LoadTexture("sample/box.png");

    //Add RenderQueue
    ZERO_SCENE_MGR->GetCurrentScene()->GetRenderManager()->AddRigidBody2D(this);

    targetCamera = ZERO_SCENE_MGR->GetCurrentScene()->FindGameObjectWithTag("MainCamera")->GetComponent<Camera2D>();
#endif

    //Add RigidBody to ColliderManager
    ZERO_SCENE_MGR->GetCurrentScene()->BindRigidBody(this->GetOwner());
}

void RigidBody2D::LateUpdate() {
    ZeroComponent::LateUpdate();
}

void RigidBody2D::CheckMass() {
//    Vec2 scale = box->GetSize();
//
//    if(mass < FLT_MAX) {
//        invMass = 1.0f / mass;
//        inertia = mass * (scale.x * scale.x + scale.y * scale.y) / 12.0f;
//        invInertia = 1.0f / inertia;
//    }
//    else {
//        invMass = 0.0f;
//        inertia = FLT_MAX;
//        invInertia = 0.0f;
//    }
}

void RigidBody2D::SetRigid(const Vec2 &width, float mass) {
    SetRigid(width.x, width.y, mass);
}

void RigidBody2D::SetRigid(float width, float height, float mass) {
    velocity = Vec2(0.0f, 0.0f);
    angularVelocity = 0.0f;
    force.Set(0.0f, 0.0f);
    torque = 0.0f;
    friction = 0.2f;

    this->width = Vec2(width, height);
    this->mass = mass;

    if (mass < FLT_MAX)
    {
        invMass = 1.0f / mass;
        I = mass * (width * width + height * height) / 12.0f;
        invI = 1.0f / I;
    }
    else
    {
        invMass = 0.0f;
        I = FLT_MAX;
        invI = 0.0f;
    }
}

void RigidBody2D::Render() {
    if(!renderTrigger) return;

    ZeroComponent::Render();
#if defined(_DEBUG) | defined(DEBUG)
    if(!shader || !targetCamera) return;

    Vec2 center = Vec2(width.x, width.y);
    Vec2 scaleCenter = transform->GetScaleCenter();
    Vec2 boxPos;
    boxPos.x = center.x * 0.5f;
    boxPos.y = center.y * 0.5f;

    glm::mat4 model = ZeroMath::MatrixTransformation(transform->GetScaleCenter(), center, transform->GetWorldRotation(), transform->GetWorldPos() - boxPos);

    shader->UseShader();
    this->shader->SetMatrix4("projection", targetCamera->projection);
    this->shader->SetMatrix4("view", targetCamera->view);
    this->shader->SetMatrix4("model", model);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glDisable(GL_BLEND);

#endif
    renderTrigger = false;
}

//float RigidBody2D::GetFriction() {
//    return box->GetFriction();
//}
