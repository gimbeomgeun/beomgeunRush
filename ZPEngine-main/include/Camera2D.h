//
// Created by EunwooSong on 2020-12-09.
//

#ifndef ZERO_ENGINE_CAMERA2D_H
#define ZERO_ENGINE_CAMERA2D_H

#include "ZeroComponent.h"
#include "Transform.h"

//-= Camera2D =-
// - 씬에서 보이는 화면의 위치, 비율 등을 설정, 관리하는 컴포넌트
class Camera2D : public ZeroComponent {
public:
    Camera2D();
    ~Camera2D();
    void Start() override;
    void Render() override;

    void Update() override;

    void SetScale(float s);//{scale = s;}
    Vec2 ConvertScreenToWorld(Vec2 screenCoords);

    glm::mat4 GetProjectionMatrix() const { return projection; }
    glm::mat4 GetViewMatrix() const { return view; }

    bool updateMatrix = true;

    float width;
    float height;

    Transform* tr;

    float scale;
    float pan_y;

    glm::mat4 projection;   //Projection Matrix
    glm::mat4 uiProjection; //UI Projection Matrix
    glm::mat4 view;         //View Matrix
};


#endif //ZERO_ENGINE_CAMERA2D_H
