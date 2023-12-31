//
// Created by EunwooSong on 2020-12-07.
//

#ifndef ZERO_ENGINE_TRANSFORM_H
#define ZERO_ENGINE_TRANSFORM_H

#include "ZeroMath.h"
#include "ZeroComponent.h"

// -= Transform =-
// - GameObject의 위치, 계층을 설정하는 함수
// - 기본적으로 포함되어있는 컴포넌트이다
class Transform :
    public Component
{
public:
    Transform() :
            localPos(0, 0),
            worldPos(0, 0),
            scaleCenter(0.5f, 0.5f),
            scale(1, 1),
            rotationCenter(0, 0),
            localRotation(0),
            worldRotation(0),
            parent(nullptr),
            isUiObject(false)
    {};
    ~Transform() {};

    void Start() override;
    void Update() override;
    void Render() override;
    void LateUpdate() override;
    void EndScene() override;
    void UpdateTransformation();

    // 로컬좌표를 가져오는 함수
    Vec2 GetLocalPos() { return localPos; }

    // 월드좌표를 가져오는 함수
    // 해당좌표는 렌더를 진행할 때 업데이트됨
    Vec2 GetWorldPos() { return worldPos; }

    // 오브젝트의 회전정보를 가져오는 함수
    float GetLocalRotation() { return localRotation; }
    float GetWorldRotation() { return worldRotation; }
    // 오브젝트의 크기를 가져오는 함수
    Vec2 GetScale() { return scale; }
    // 오브젝트의 중심 축을 가져오는 함수
    Vec2 GetScaleCenter() { return scaleCenter;}
    // 해당 오브젝트의 자식 오브젝트를 가져오는 함수
    Transform* GetChild(int index) { return children[index]; }
    // 해당 오브젝트의 부모 오브젝트를 가져오는 함수
    Transform* GetParent() { return parent; }
    // 모델 뷰의 메트릭스를 가져오는 함수
    const glm::mat4& GetMatrix() { return mat; }

    // 오브젝트의 부모를 설정하는 함수
    void SetParent(Transform* iter)         { parent = iter; };
    // 오브젝트의 포지션을 설정하는 함수(로컬)
    void SetPosition(double x, double y)    { this->localPos = Vec2(x, y); }
    void SetPosition(Vec2 localPos)         { this->localPos = localPos; };
    // 오브젝트의 회정정보를 설정하는 함수 
    void SetRotationRadians(float z)        { this->localRotation = z; }
    void SetRotationDegree(float z)         { this->localRotation = glm::radians(z); }
    void SetRotationCenter(Vec2 rc)         { rotationCenter = rc; }
    // 오브젝트의 크기정보를 설정하는 함수
    void SetScale(Vec2 scale)               { this->scale = scale; }
    void SetScale(double x, double y)       { this->scale = Vec2(x, y); }
    // 오브젝트의 중심 축을 가져오는 함수
    void SetScaleCenter(Vec2 sc)        { scaleCenter = sc; }
    void SetScaleCenter(float x, float y)        { scaleCenter.x = x; scaleCenter.y = y; }
    // UI오브젝트는 다른 좌표계를 사용하기 때문에 구분을 위함
    void SetIsUiObject(bool isUiObject) { this->isUiObject = isUiObject; }

    // 입력된 회전율만큼 추가하는 함수
    void Rotate(float rotation) { this->localRotation += rotation; }
    // 입력된 위치만큼 추가하는 함수
    void Translate(Vec2 pos) { localPos += pos; }
    // 입력된 자식을 추가하는 함수
    void AddChild(Transform* iter);
    // 입력된 자식을 제거하는 함수
    void PopChild(Transform* iter);
    // 계산된 모델뷰에서 월드뷰로 변환하기 위한 함수
    void MulMatrix(glm::mat4 mat) { if(!isUiObject) this->mat *= mat; }

protected:
    //Position, Direction, Angles, Quaternion, scale, matrix
    Vec2 localPos;
    Vec2 worldPos;          //ReadOnly
    Vec2 scaleCenter;
    Vec2 scale;
    Vec2 rotationCenter;
    float localRotation;
    float worldRotation;    //ReadOnly
    glm::mat4 mat;          //ModelView

    bool isUiObject;

    Transform* parent;
    std::vector<Transform*> children;
};


#endif //ZERO_ENGINE_TRANSFORM_H
