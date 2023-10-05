//
// Created by EunwooSong on 2020-12-01.
//

#include "stdafx.h"
#include "ZeroInputMgr.h"
#include "ZeroSystem.h"

void ZeroInputMgr::Initialize() {
    ZeroMemory(keyInput,    sizeof(keyInput));
    ZeroMemory(lastInput,   sizeof(lastInput));

    ZeroMemory(mouseInput,    sizeof(mouseInput));
    ZeroMemory(mouselastInput,   sizeof(mouselastInput));

    ZeroMemory(textBuffer, sizeof(textBuffer));
    ZeroMemory(charBuffer, sizeof(charBuffer));

#if  WIN32
    // Get HWND
    //handle = WindowFromDC(wglGetCurrentDC());
    handle = glfwGetWin32Window(windowObj->GetWindow());

    if (handle == NULL)
        assert("glfwGetWin32Window Error");
    else {
        // Send the window to now use out event handler...
        currentWndProc = (WNDPROC) SetWindowLongPtr(handle, GWLP_WNDPROC, (LONG_PTR)&winProc);
    }
#endif //  WIN32
}

void ZeroInputMgr::Update() {
    std::copy(keyInput, keyInput + 512, lastInput);

    glfwGetCursorPos(windowObj->GetWindow(), &cursorPos.x, &cursorPos.y);

    for(int i = 8; i < 512; i++) {
        lastInput[i] = keyInput[i];
        keyInput[i] = glfwGetKey(windowObj->GetWindow(), i);
    }

    for(int i = 0; i < 8; i ++) {
        lastInput[i] = keyInput[i];
        keyInput[i] = glfwGetMouseButton(windowObj->GetWindow(), i);
    }
}

KeyState ZeroInputMgr::GetKeyState(int vk) {

    if(GetKeyDown(vk))      return KeyState::DOWN;
    else if(GetKeyUp(vk))   return KeyState::UP;
    else if(GetKey(vk))     return KeyState::PRESS;
    else                    return KeyState::NONE;
}

bool ZeroInputMgr::GetKeyDown(int vk) {
    if(lastInput[vk] != GLFW_PRESS)
        if(keyInput[vk] == GLFW_PRESS) return true;
    return false;
}

bool ZeroInputMgr::GetKey(int vk) {
    if(GetKeyDown(vk)) return true;
    else if(keyInput[vk] == GLFW_PRESS) return true;
    return false;
}

bool ZeroInputMgr::GetKeyUp(int vk) {
    if(lastInput[vk] != GLFW_RELEASE)
        if(keyInput[vk] == GLFW_RELEASE) return true;
    return false;
}
ZeroVec2 ZeroInputMgr::GetCursorPos() {
    return cursorPos;
}

std::string ZeroInputMgr::GetTextInput()
{
    return std::string(textBuffer, textBuffer + strlen(textBuffer));
}

void ZeroInputMgr::ClearCharacterBuffer()
{
    ZeroMemory(textBuffer, sizeof(textBuffer));
    ZeroMemory(charBuffer, sizeof(charBuffer));
}

#if WIN32 //For Windows
LRESULT ZeroInputMgr::winProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    // IME 처리 확인 이후 기존의 winProc 실행
    if(ZERO_INPUT_MGR->GetIsReadyToInput())
        GetTextIME(hwnd, Msg, wParam, lParam);

    return CallWindowProc(ZERO_INPUT_MGR->currentWndProc, ZERO_INPUT_MGR->handle, Msg, wParam, lParam);
}

int ZeroInputMgr::GetTextIME(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int len;
    HIMC m_hIMC = NULL;     // IME 핸들

    int textBufferLen = strlen(ZERO_INPUT_MGR->textBuffer);

    switch (msg) {
    case WM_IME_COMPOSITION:    // 영어, 한자 등을 제외한 입력처리 진행
        m_hIMC = ImmGetContext(hWnd);

        if (lParam & GCS_RESULTSTR) { // 현재 완성된 글자가 있음
            if ((len = ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, NULL, 0)) > 0) {
                ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, ZERO_INPUT_MGR->charBuffer, len);
                ZERO_INPUT_MGR->charBuffer[len] = 0;
                strcpy(ZERO_INPUT_MGR->textBuffer + textBufferLen, ZERO_INPUT_MGR->charBuffer);
                memset(ZERO_INPUT_MGR->charBuffer, 0, 256);
            }
        }
        else if (lParam & GCS_COMPSTR) { // 현재 글자를 조합중

            // 조합중인 길이를 얻음
            len = ImmGetCompositionString(m_hIMC, GCS_COMPSTR, NULL, 0);
            ImmGetCompositionString(m_hIMC, GCS_COMPSTR, ZERO_INPUT_MGR->charBuffer, len);
            ZERO_INPUT_MGR->charBuffer[len] = 0;
        }

        ImmReleaseContext(hWnd, m_hIMC);    // IME 핸들 반환
        return 0;

    case WM_CHAR: // 아스키 코드 입력 처리
        if (wParam < 128) {
            if (wParam == 8) { // Backspace...
                if (textBufferLen > 0)
                    ZERO_INPUT_MGR->textBuffer[textBufferLen - 1] = 0;
            }
            else
                ZERO_INPUT_MGR->textBuffer[strlen(ZERO_INPUT_MGR->textBuffer)] = wParam;
        }
        return 0;
    }

    return 1;
}
#endif