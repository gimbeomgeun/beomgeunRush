//
// Created by EunwooSong on 2020-11-27.
//

#include "stdafx.h"
#include "ZeroWindow.h"
#include "ZeroLogger.h"
#include "ZeroSystem.h"

ZeroWindow::ZeroWindow() {
    
}

ZeroWindow::~ZeroWindow() {
    
}

void ZeroWindow::RegisterWindow(std::string appName, int width, int height, bool isFullscreen, float R, float G, float B) {
    samples = 4;
    closeWindow = false;

    if (this->appName != nullptr)
        SAFE_DELETE(this->appName);

    this->appName = new std::string(appName);
    this->width = width;
    this->height = height;
    this->isFullScreen = isFullscreen;
    r = R; g= G; b = B;
}

void ZeroWindow::Initialize() {
    if( !glfwInit() )
    {
        ZeroLogger::Error("Initialize GLFW failed");
        exit(-1);
    }

//    glfwWindowHint(GLFW_SAMPLES, samples);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, (*appName).c_str(), isFullScreen ? glfwGetPrimaryMonitor() : NULL, NULL);
    if(window == NULL) {
        ZeroLogger::Error("Create window failed");
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;

    if(glewInit() != GLEW_OK) {
        ZeroLogger::Error("Initialize GLEW failed");
        exit(-1);
    }

    glfwSwapInterval(1);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_STENCIL_TEST);
    glClearColor(r, g, b, 1.0f);
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

bool ZeroWindow::ChangeBuffer() {
    glfwSwapBuffers(window);
    glfwPollEvents();

    return true;
}

void ZeroWindow::Release() {
    //glfwDestroyWindow(window);
    if(!closeWindow)
        glfwDestroyWindow(window);
    glfwTerminate();

    SAFE_DELETE(appName);
}

bool ZeroWindow::CheckWindowClose() {
    return glfwWindowShouldClose(window);
}

void ZeroWindow::CloseWindow() {
    glfwDestroyWindow(window);
    closeWindow = true;
}

void ZeroWindow::ChangeFullScreen() {
    Release();

    isFullScreen = !isFullScreen;
    Initialize();
}

