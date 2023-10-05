//
// Created by EunwooSong on 2020-12-06.
//
#include "stdafx.h"
#include "ZeroSystem.h"
#include "ZeroSceneMgr.h"
#include "ZeroScene.h"
#include "ZeroComponent.h"

ZeroSceneMgr::~ZeroSceneMgr() {
    Release();
}

ZeroScene *ZeroSceneMgr::GetCurrentScene() {
    return currentScene;
}

void ZeroSceneMgr::Start() {
    if (currentScene) currentScene->Start();
}

void ZeroSceneMgr::Update() {
    if (currentScene) currentScene->Update();
}

void ZeroSceneMgr::LateUpdate() {
    if (currentScene) currentScene->LateUpdate();
}

void ZeroSceneMgr::Render() {
    if (currentScene) {
        rendererObj->Render();
        currentScene->Render();
    }
}

void ZeroSceneMgr::EndScene() {
    if (currentScene) {
        rendererObj->EndScene();
        currentScene->EndScene();
    }

    if (nextScene) {
        SAFE_DELETE(currentScene);
        currentScene = nextScene;
        nextScene = nullptr;

        currentScene->Init();
        changed = true;
        count = 2;

        rendererObj->ChangeScene(currentScene->GetMainCamera());
    }

    if(changed) {
        if(--count < 0)
            changed = false;
    }
}

void ZeroSceneMgr::FixedUpdate()
{
    if (currentScene) currentScene->FixedUpdate();
}

void ZeroSceneMgr::ChangeScene(ZeroScene *scene) {
    if (!scene) {
        throw std::runtime_error("This is not supported type(Scene Manager)");
    }

    if (currentScene) {
        nextScene = scene;
    }
    else {
        currentScene = scene;
        currentScene->Init();

        rendererObj->ChangeScene(currentScene->GetMainCamera());
    }
}

void ZeroSceneMgr::Release() {
    if(currentScene) SAFE_DELETE(currentScene);
    if(nextScene) SAFE_DELETE(nextScene);
}
