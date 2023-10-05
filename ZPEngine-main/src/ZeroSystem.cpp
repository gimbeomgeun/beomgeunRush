//
// Created by EunwooSong on 2020-12-06.
//
#include "stdafx.h"
#include "ZeroSystem.h"
#include "ZeroWindow.h"
#include "ZeroInputMgr.h"
#include "ZeroTimeMgr.h"
#include "ZeroSceneMgr.h"
#include "ZeroSoundDevice.h"
#include "ZeroResourcesManager.h"

ZeroSystem::ZeroSystem() {
    ZERO_LOGGER::Info("ZeroSystem Created...");

    window = new ZeroWindow();
    input = new ZeroInputMgr(window);
    time = new ZeroTimeMgr();
    sound = new ZeroSoundDevice();
    resource = new ZeroResourcesManager();
    renderer = new ZeroRenderManager(window, resource);
    scene = new ZeroSceneMgr(renderer);
}

ZeroSystem::~ZeroSystem() {

}

ZeroSystem *ZeroSystem::Instance() {
    static ZeroSystem instance;
    return &instance;
}

//씬의 초기화를 진행한다
//(appName : 프로그램 이름,
//width : 가로 픽셀, height : 세로 픽셀,
//isFullScreen : 전체화면, r,g,b : 배경 색상 값)
void ZeroSystem::RegisterSystem(std::string appName,
    int width, int height, bool isFullScreen,
    float r, float g, float b)
{
#if defined(DEBUG) | defined(_DEBUG)
    isDebug = true;
#endif
    isDebug = false;

    if (this->appName != nullptr)
        SAFE_DELETE(this->appName);
    this->appName = new std::string(appName);
    this->width = width;
    this->height = height;
    this->isFullScreen = isFullScreen;

    ZERO_LOGGER::Info("ZeroSystem Registered...");
    window->RegisterWindow(appName, width, height, isFullScreen, r, g, b);
}
//Window, Input, Resources, Time, Renderer 초기화 진행
void ZeroSystem::InitializeSystem() {
    window->Initialize();
    input->Initialize();
    time->Initialize();
    resource->Initialize();
    renderer->Initialize();

    //Sound Device Initialize
    sound->SetAttunation(AL_INVERSE_DISTANCE_CLAMPED);
    sound->SetLocation(0.f, 0.f, 0.f);
    sound->SetOrientation(0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
}
//게임 루프 진행
int ZeroSystem::MainLoop() {
    while (!window->CheckWindowClose()) {
        window->ChangeBuffer();
        Start();        // Update 전 한 번 실행
        Update();       // 매 프레임 마다 실행
        LateUpdate();   // Update 다음에 실행
        Render();       // 렌더 진행
        EndScene();     // 씬 마무리(지울 게임 오브젝트 정리 용도)

        FixedUpdate();  // 초당 60번 호출됨

    }

    Release();          // 언로드 진행

    return 0;
}

void ZeroSystem::Start() {
    time->Update();
    input->Update();

    scene->Start();
}

void ZeroSystem::Update() {
    scene->Update();
}
    
void ZeroSystem::LateUpdate() {
    scene->LateUpdate();
}

void ZeroSystem::Render() {
    scene->Render();
}

void ZeroSystem::EndScene() {
    scene->EndScene();
}

void ZeroSystem::FixedUpdate()
{
    scene->FixedUpdate();
}

void ZeroSystem::Release() {
    scene->Release();
    resource->Release();
    window->Release();
    sound->ReleaseDevice();

    // Safe Delete All Managers...
    SAFE_DELETE(appName);
    SAFE_DELETE(window);
    SAFE_DELETE(input);
    SAFE_DELETE(time);
    SAFE_DELETE(sound);
    SAFE_DELETE(resource);
    SAFE_DELETE(renderer);
    SAFE_DELETE(scene);
}
