//
// Created by EunwooSong on 2020-12-09.
//

#include "stdafx.h"
#include "ZeroSystem.h"
#include "ZeroGameObject.h"
#include "Camera2D.h"
#include "ZeroMath.h"

Camera2D::Camera2D() : updateMatrix(false), scale(500) {
    height =  static_cast<float>(ZERO_WINDOW->GetHeight());
     width =   static_cast<float>(ZERO_WINDOW->GetWidth());
    uiProjection = glm::ortho(0.0f, width, height, 0.0f);

    float aspect = width / height;
    pan_y = 8.0f;

    projection = glm::ortho(-scale * aspect, scale * aspect, -scale + pan_y, scale + pan_y, -1.0f, 1.0f);
}

Camera2D::~Camera2D() {

}

void Camera2D::Start() {
    ZeroComponent::Start();

    GetOwner()->SetTag("MainCamera");
    tr = GetOwner()->transform;
}

void Camera2D::Render() {
    ZeroComponent::Render();

    view = ZeroMath::MatrixTransformation(tr->GetScaleCenter(), tr->GetScale(), tr->GetWorldRotation(), tr->GetWorldPos() * -1);
}

void Camera2D::Update() {
    ZeroComponent::Update();

//    Vec2 pos = tr->GetLocalPos();
//
//    glm::vec3 translate(-pos.x + width / 2, -pos.y + height / 2, 0.0f);
//    view = glm::translate(projection, translate);
//
//    //Camera Scale
//    glm::vec3 scale(this->scale, this->scale, 0.0f);
//    view = glm::scale(glm::mat4(1.0f), scale) * view;
}

Vec2 Camera2D::ConvertScreenToWorld(Vec2 screenCoords) {
    // Invert Y direction
    screenCoords.y = height - screenCoords.y;
    // Make it so that 0 is the center
    screenCoords -= glm::vec2(width / 2, height / 2);
    // Scale the coordinates
    screenCoords /= scale;
    // Translate with the camera position
    screenCoords += tr->GetLocalPos();
    return screenCoords;
}

void Camera2D::SetScale(float s) {
    scale = s;

    height =  static_cast<float>(ZERO_WINDOW->GetHeight());
    width =   static_cast<float>(ZERO_WINDOW->GetWidth());
    uiProjection = glm::ortho(0.0f, width, height, 0.0f);

    float aspect = width / height;
    pan_y = 8.0f;

    projection = glm::ortho(-scale * aspect, scale * aspect, -scale + pan_y, scale + pan_y, -1.0f, 1.0f);
}
