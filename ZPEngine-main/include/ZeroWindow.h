//
// Created by EunwooSong on 2020-11-27.
//

#ifndef ZERO_ENGINE_ZEROWINDOW_H
#define ZERO_ENGINE_ZEROWINDOW_H

#include "string"

class ZeroWindow {
public:
    ZeroWindow();
    ~ZeroWindow();

    void RegisterWindow(std::string appName, int width, int height, bool isFullscreen = false, float R = 0.0f, float G = 0.0f, float B = 0.0f);
    void Initialize();

    bool ChangeBuffer();
    bool CheckWindowClose();
    void Release();
    void CloseWindow();
    void ChangeFullScreen();

    GLFWwindow* GetWindow() const { return window; }
    int GetWidth() { return width; }
    int GetHeight() { return height; }
    std::string& GetApplicationName() { return *appName; }

    void SetAntiAliasing(int samples) { this->samples = samples; }

private:
    GLFWwindow* window;

    int width;
    int height;
    bool isFullScreen;
    std::string * appName = nullptr;

    int samples;
    bool closeWindow;

    float r, g, b;
};


#endif //ZERO_ENGINE_ZEROWINDOW_H
