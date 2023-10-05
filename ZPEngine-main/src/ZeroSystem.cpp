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

//���� �ʱ�ȭ�� �����Ѵ�
//(appName : ���α׷� �̸�,
//width : ���� �ȼ�, height : ���� �ȼ�,
//isFullScreen : ��üȭ��, r,g,b : ��� ���� ��)
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
//Window, Input, Resources, Time, Renderer �ʱ�ȭ ����
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
//���� ���� ����
int ZeroSystem::MainLoop() {
    while (!window->CheckWindowClose()) {
        window->ChangeBuffer();
        Start();        // Update �� �� �� ����
        Update();       // �� ������ ���� ����
        LateUpdate();   // Update ������ ����
        Render();       // ���� ����
        EndScene();     // �� ������(���� ���� ������Ʈ ���� �뵵)

        FixedUpdate();  // �ʴ� 60�� ȣ���

    }

    Release();          // ��ε� ����

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
