//
// Created by EunwooSong on 2020-12-06.
//

#ifndef ZERO_ENGINE_ZEROTIMEMGR_H
#define ZERO_ENGINE_ZEROTIMEMGR_H

class GLFWwindow;

class ZeroTimeMgr {
public:
    ZeroTimeMgr();
    ~ZeroTimeMgr();

    void Initialize();
    void Update();
    double GetDeltaTime();
    float GetDeltaTimeF();
private:
    GLFWwindow* window;
    double deltaTime;
    double nowTime;
    double lastTime;
};


#endif //ZERO_ENGINE_ZEROTIMEMGR_H
