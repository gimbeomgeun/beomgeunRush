//
// Created by EunwooSong on 2020-12-06.
//

#include "stdafx.h"
#include "ZeroTimeMgr.h"
#include "ZeroSystem.h"

ZeroTimeMgr::ZeroTimeMgr() {

}

ZeroTimeMgr::~ZeroTimeMgr() {

}

void ZeroTimeMgr::Initialize() {
    deltaTime = 0.0f;
    lastTime = 0.0f;
    nowTime = 0.0f;
}

void ZeroTimeMgr::Update() {
    nowTime = glfwGetTime();
    deltaTime = nowTime - lastTime;
    lastTime = nowTime;
}

double ZeroTimeMgr::GetDeltaTime() {
    return deltaTime;
}

float ZeroTimeMgr::GetDeltaTimeF() {
    return static_cast<float>(deltaTime);
}
