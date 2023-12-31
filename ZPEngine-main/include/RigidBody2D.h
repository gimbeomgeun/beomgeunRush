//
// Created by EunwooSong on 2020-12-12.
//

#ifndef ZERO_ENGINE_RIGIDBODY2D_H
#define ZERO_ENGINE_RIGIDBODY2D_H


#include "ZeroComponent.h"
#include "ZeroMath.h"
#include "ZeroDefine.h"
#include "ZeroWrapper.h"

class Camera2D;
class Transform;
class BoxCollider;

// -= RigidBody2D =-
// - 물리 연산에 필요한 변수들을 설정하고 저장하는 컴포넌트
// - 실제 모든 물리연산은 ZeroColliderManager에서 진행한다
// - 박스 콜라이더가 기본 내장되어 있음
class RigidBody2D : public Component {
public:
    RigidBody2D();
    ~RigidBody2D() = default;

    void Start() override;
    void LateUpdate() override;
    void Render() override;

	// Debug 빌드시 박스의 경계면을 출력하기 위함
    void StartRender() {renderTrigger = true;}
	
	// 물리 연산을 진행하기위해 필요한 기본 설정을 진행하는 함수
    void SetRigid(const Vec2& width, float mass);
    void SetRigid(float width, float height, float mass);

	// 속도를 설정하는 함수
    void SetVelocity(float x, float y) { velocity.x = x; velocity.y = y; }
    void SetVelocity(Vec2 v) { velocity.x = v.x; velocity.y = v.y; }
    void SetAngularVelocity(float angle) { if(!freezeRotation) angularVelocity = angle; }

    // 힘을 설정하는 함수
    void SetForce(float x, float y) { force.x = x; force.y = y; }
    void SetForce(Vec2 v) { force.x = v.x; force.y = v.y; }

    // 토크를 설정하는 함수
    void SetTorque(float t) { torque = t; }
    // 질량을 설정하는 함수
    void SetMass(float mass) { this->mass = mass; }//CheckMass(); }

    // 힘을 추가하는 함수
    void AddForce(Vec2 v) { force += v; }

    Vec2 GetVelocity() const { return velocity; }
    float GetAngularVelocity() const { return angularVelocity; }
    Vec2 GetForce() const { return force; }
    float GetTorque() const { return torque; }
    float GetMass() const { return mass; }
    float GetInvMass() const { return invMass; }
    float GetInertia() const { return I; }
    float GetInvInertia() const { return invI; }
    float GetFriction() const { return friction; }

    void SetVAO(GLuint vao) { this->vao = vao; };
    void SetVBO(GLuint vbo) { this->vbo = vbo; };

public:
    void CheckMass();

    Transform* transform;

#if defined(DEBUG) | defined(_DEBUG)
    //For Test

    GLuint vao;
    GLuint vbo;

    ZeroShader* shader;
    Camera2D* targetCamera;
#endif

    Vec2 velocity;                  //속도
    float angularVelocity;         //회전 속도

    Vec2 force;                     //힘
    float torque;                   //토크

    Vec2 width;

    float friction;

    float mass, invMass;          //질량
    float I, invI;                     //관성

    Color debugColor;

    bool renderTrigger;
    bool isTrigger = false;
    bool freezeRotation = false;
};

#endif //ZERO_ENGINE_RIGIDBODY2D_H
