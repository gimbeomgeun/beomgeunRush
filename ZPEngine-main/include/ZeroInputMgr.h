//
// Created by EunwooSong on 2020-12-01.
//

#ifndef ZERO_ENGINE_ZEROINPUTMGR_H
#define ZERO_ENGINE_ZEROINPUTMGR_H

#include "ZeroMath.h"

//KeyInput : https://www.glfw.org/docs/3.3/group__keys.html#gabf48fcc3afbe69349df432b470c96ef2
//MouseInput : https://www.glfw.org/docs/3.3/group__buttons.html#ga181a6e875251fd8671654eff00f9112e

#define INPUT_MOUSE_LBUTTON GLFW_MOUSE_BUTTON_LEFT
#define INPUT_MOUSE_RBUTTON GLFW_MOUSE_BUTTON_RIGHT

#include "stdafx.h"

class ZeroVec2;
class ZeroWindow;

enum class KeyState {
    NONE, PRESS, UP, DOWN
};

class ZeroInputMgr {
public:
    ZeroInputMgr(ZeroWindow* windowObj) : isReadyToInput(false) { this->windowObj = windowObj; };
    ~ZeroInputMgr() = default;

    void Initialize();

    void Update();

#if WIN32
    static LRESULT CALLBACK winProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    // IME ó��
    static int GetTextIME(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
#endif

    KeyState GetKeyState(int vk);
    bool GetKeyDown(int vk);
    bool GetKey(int vk);
    bool GetKeyUp(int vk);
    ZeroVec2 GetCursorPos();

    // �Էµ� �ؽ�Ʈ ���� ������
    std::string GetTextInput();

    // �ؽ�Ʈ �Է� ���� ����
    void SetIsReadyToInput(bool isReadyToInput) { this->isReadyToInput = isReadyToInput; }
    bool GetIsReadyToInput() { return isReadyToInput; }

    // Character Buffer�� ���
    void ClearCharacterBuffer();

private:
    int keyInput[512];
    int lastInput[512];

    int mouseInput[8];
    int mouselastInput[8];

    ZeroVec2 cursorPos;

    bool isReadyToInput;

    // ���ڵ��� ����
    char textBuffer[512];

    // �������� ���ڵ��� ����
    char charBuffer[256];

#if WIN32
    // ������ IME ó���� ����
    HWND handle;
    WNDPROC currentWndProc;
#endif

    ZeroWindow* windowObj;
};


#endif //ZERO_ENGINE_ZEROINPUTMGR_H
