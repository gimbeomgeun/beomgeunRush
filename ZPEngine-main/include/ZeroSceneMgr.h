//
// Created by EunwooSong on 2020-12-06.
//

#ifndef ZERO_ENGINE_ZEROSCENEMGR_H
#define ZERO_ENGINE_ZEROSCENEMGR_H

class ZeroScene;
class ZeroRenderManager;

class ZeroSceneMgr {
public:
    ZeroSceneMgr(ZeroRenderManager* rendererObj) : currentScene(nullptr), nextScene(nullptr), rendererObj(rendererObj) {};
    ~ZeroSceneMgr();

    ZeroScene* GetCurrentScene();

    void Start();
    void Update();
    void LateUpdate();
    void Render();
    void EndScene();
    void FixedUpdate();

    void Release();

    void ChangeScene(ZeroScene* scene);

private:
    ZeroScene* currentScene;
    ZeroScene* nextScene;

    ZeroRenderManager* rendererObj;

    bool changed = false;
    int count = 0;
};


#endif //ZERO_ENGINE_ZEROSCENEMGR_H
