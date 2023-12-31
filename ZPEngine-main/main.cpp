#include "ZeroGameObject.h" //게임 오브젝트 생성을 위함
#include "ZeroComponent.h"  //컴포넌트 생성을 위함
#include "stdafx.h"
#include "ZeroSystem.h"
#include "ZeroScene.h"
#include "ZeroSceneMgr.h"
#include "Sprite2DRenderer.h"
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include "RigidBody2D.h"

class HelloScene;
class AnimScene;
class GameManager;
class PlayerMovement;
class GiNi;
class GiNiMove;
class CheckCollistion;
class Anim;

GameObject* giniInst(float x)
{
    GameObject* giniPrefab = new GameObject;
    giniPrefab->AddComponent<GiNi>();
    giniPrefab->AddComponent<Sprite2DRenderer>();
    giniPrefab->GetComponent<Sprite2DRenderer>()->SetTexture("sample/gini.png");
    giniPrefab->transform->SetPosition(x, 500);
    giniPrefab->AddComponent<GiNiMove>();
    giniPrefab->AddComponent<RigidBody2D>()->SetRigid(200, 200, 10);
    giniPrefab->SetTag("GiNi");
    giniPrefab->GetComponent<RigidBody2D>()->isTrigger = true;

    return giniPrefab;
}

GameObject* beomInst(float x, float y)
{
    GameObject* beom = new GameObject;
    beom->AddComponent<Sprite2DRenderer>(); //게임 오브젝트에 Sprite2DRenderer를 등록합니다.
    beom->GetComponent<Sprite2DRenderer>()->SetTexture("sample/beomgeun.png"); // "../Resources/sample/Player.png"를 렌더링할 텍스쳐로 설정합니다.
    beom->transform->SetPosition(x, y);
    beom->transform->SetScale(0.3, 0.3);
    beom->GetComponent<Sprite2DRenderer>()->SetColor(x / 30, y / 30, (x + y) / 30, 0.9);
    return beom;
}

class AnimScene : public Scene {
public:
    GameObject* background;
    void Init() {
        Scene::Init();
        background = new GameObject;
        background->AddComponent<Sprite2DRenderer>();
        background->GetComponent<Sprite2DRenderer>()->SetTexture("sample/whiteimage500.png");
        background->transform->SetScale(3.0, 3.0);
        for (int i = -16; i < 18; i++)
        {
            for (int j = -124; j < 24; j++)
            {
                background->transform->AddChild(beomInst(j * 60, i * 60)->transform);
                //beomInst(j * 60, i * 60);
            }
        }
        background->AddComponent<Anim>();
    }

};
//HelloScene 클래스 생성
class HelloScene : public Scene { //Scene말고 ZeroScene을 상속받아도 사용이 가능합니다.
public:
    void Init() {
        Scene::Init(); //필수
        ZERO_LOGGER::Debug("Initialize Scene");
        GameObject* gameManager = new GameObject();
        gameManager->AddComponent<GameManager>();
        GameObject* player = new GameObject(); //게임 오브젝트 생성, 자동으로 씬에 등록됩니다. (ZeroGameObject를 생성해도 사용이 가능합니다)
        player->AddComponent<PlayerMovement>(); //게임 오브젝트에 PlayerMovement를 등록합니다.
        player->AddComponent<Sprite2DRenderer>(); //게임 오브젝트에 Sprite2DRenderer를 등록합니다.
        player->GetComponent<Sprite2DRenderer>()->SetTexture("sample/beomgeun.png"); // "../Resources/sample/Player.png"를 렌더링할 텍스쳐로 설정합니다.
        player->AddComponent<RigidBody2D>()->SetRigid(200, 200, FLT_MAX);
        player->GetComponent<RigidBody2D>()->isTrigger = true;
        player->AddComponent<CheckCollistion>();
        m_ColliderManager.get()->Init(Vec2(0, -100), 10);
    }
};

class GameManager : public Component {
public:
    void Start() {
        Component::Start();
    }

    void Update() {
        Component::Update();
        int r = rand() % 50;
        if (r == 0)
        {
            giniInst(rand() % 1280 - 640);
        }

    }

};

class Anim : public Component {
public:
    Vec2 moveDir = Vec2(1.0f, 1.0f);
    float moveSpeed = 70.0f;
    float time;
    const char* a = "0.0f";
    int animN = 0;
    Transform* tr;
    void Start() {
        Component::Start();
        tr = GetOwner()->GetComponent<Transform>();
        time = 0.0f;
    }
    void Update() {
        Component::Update();
        time += ZERO_TIME_MGR->GetDeltaTimeF();
        if (tr->GetLocalPos().x < 500 && animN == 0)
        {
            moveDir.x = 1;
            moveDir.y = 0;
            tr->Translate(moveDir * moveSpeed * time);
        }
        if (tr->GetLocalPos().y > -250 && animN == 1)
        {
            moveDir.x = 0;
            moveDir.y = -1;
            tr->Translate(moveDir * moveSpeed * time);
        }
        if (tr->GetLocalPos().x > -400 && animN == 2)
        {
            moveDir.x = -1;
            moveDir.y = 0;
            tr->Translate(moveDir * moveSpeed * time);
        }
        if (tr->GetLocalPos().x >= 500)
            animN = 1;
        if (tr->GetLocalPos().y <= -250)
            animN = 2;
        if (tr->GetLocalPos().x < 500 && tr->GetLocalPos().x <= -400)
            ZERO_SCENE_MGR->ChangeScene(new HelloScene());
    }
};

//PlayerMoement 클래스 생성
class PlayerMovement : public Component { //Component말고 ZeroComponent를 상속받아도 사용이 가능합니다.
public:
    void Start() {
        Component::Start();

        //컴포넌트를 게임 오브젝트에서 가져오기 위해서는 GetOwner()로 오브젝트를 불러와야한다.
        tr = GetOwner()->GetComponent<Transform>();
        tr->SetPosition(Vec2(0, -300));
    }
    void Update() {
        Component::Update();

        //키 입력에 따른 입력 방향을 저장하기위해 선언하였습니다.
        Vec2 moveDir = Vec2(0, 0);

        //키 입력에 따라 이동할 방향을 저장합니다.
        if (ZERO_INPUT_MGR->GetKey('A'))
        {
            moveDir.x = -1; // A를 누르면 왼쪽으로 이동합니다.
            tr->SetScale(-1, 1);
        }
        if (ZERO_INPUT_MGR->GetKey('D'))
        {
            moveDir.x = 1; // D를 누르면 오른쪽으로 이동합니다.
            tr->SetScale(1, 1);
        }

        // 이동 방향에 속도를 곱하여 이동을 진행합니다. (매 프레임마다 이동 방향으로 10 이동)
        tr->Translate(moveDir * moveSpeed * ZERO_TIME_MGR->GetDeltaTimeF());
    }

    float moveSpeed = 500.0f;
    Transform* tr;
};

class GiNiMove : public Component {
public:
    void Start() {
        Component::Start();
        tr = GetOwner()->GetComponent<Transform>();
        rigid = GetOwner()->GetComponent<RigidBody2D>();
        rigid->SetVelocity(moveDir);
    }
    void Update() {
        /*Component::Update();
        tr = GetOwner()->GetComponent<Transform>();
        tr->Translate(moveDir * moveSpeed*ZERO_TIME_MGR->GetDeltaTimeF()*9.8f);
        if (tr->GetLocalPos().y <= -100)
        {
            this->Destroy();
        }*/

        rigid = GetOwner()->GetComponent<RigidBody2D>();
        rigid->AddForce(rigid->GetVelocity() * moveSpeed);
    }
    float moveSpeed = 30.0f;
    Vec2 moveDir = Vec2(0, -1);
    RigidBody2D* rigid;
    Transform* tr;
};

class GiNi : public Component { //Component말고 ZeroComponent를 상속받아도 사용이 가능합니다.
public:
    void Start() {
        Component::Start();
        tr = GetOwner()->GetComponent<Transform>();
        tr->SetScale(0.3, 0.3);
        tr->SetPosition(tr->GetLocalPos().x, 500);
        int r = rand() % 2;
        if (r)
        {
            ZERO_LOGGER::Debug("Nieun inst");
            tr->SetScale(-tr->GetScale().x, -tr->GetScale().y);
        }
    }
    Transform* tr;
};


class CheckCollistion : public Component {
public:
    float deltatime;
    float time;
    void Start() {
        Component::Start(); //컴포넌트 생성시 필수..!
        sprite = GetOwner()->GetComponent<Sprite2DRenderer>();
        time = 0.0f;
    }

    void Update() {
        Component::Update();
        deltatime = ZERO_TIME_MGR->GetDeltaTimeF();
    }

    //충돌이 진행됐을 때 수행할 로직을 작성합니다.
    void OnCollisionStay(ZeroGameObject* coll) {
        if (coll->GetTag().compare("GiNi") == 0) //충돌한 객체의 tag가 floor라면 아래 로직을 수행합니다.
        {
            time += deltatime;
            sprite->SetColor(100, 0, 0, 10);
            ZERO_LOGGER::Debug("coll");
            coll->Destroy(coll);
            ZERO_SCENE_MGR->ChangeScene(new AnimScene());
            time = 0.0f;
        }
    }
    Sprite2DRenderer* sprite;
};

int main() {
    bool isFullScreen = false;

    //시스템의 기본적인 값을 설정해준다
    ZERO_SYSTEM->RegisterSystem("ZeroEngine",
        1280, 720,
        isFullScreen,
        255.0f, 255.0f, 255.0f);

    //게임 엔진의 초기화를 진행한다
    ZERO_SYSTEM->InitializeSystem();

    //시작 씬을 설정해준다
    ZERO_SCENE_MGR->ChangeScene(new HelloScene());

    ZERO_SYSTEM->MainLoop();

    _CrtDumpMemoryLeaks();
    return 0;
}