//
// Created by EunwooSong on 2020-12-27.
//

#ifndef ZERO_ENGINE_UITEXTRENDERER_H
#define ZERO_ENGINE_UITEXTRENDERER_H

#include "ZeroComponent.h"
#include "ZeroWrapper.h"
#include "Transform.h"

// -= UIRenderer =-
// - UI 좌표계에 텍스쳐를 출력하는 함수
// - 한국어 출력은 잘 모르겠어요
class UITextRenderer : public Component {
public:
    UITextRenderer() : renderInScene(false), text("TEST TEXT"), color(1.0f, 1.0f, 1.0f, 1.0f), textScale(1.0f), renderTrigger(false) {}
    ~UITextRenderer() = default;

    void Start() override;
    void StartRender() {renderTrigger = true; }
    void Render() override;

    // 출력할 텍스쳐를 설정하는 함수
    UITextRenderer* SetText(std::string text, int len = -1);
    // 출력할 텍스쳐의 크기를 설정하는 함수
    UITextRenderer* SetTextScale(float scale);
    // 출력할 텍스쳐의 색을 설정하는 함수
    UITextRenderer* SetTextColor(float r, float g, float b, float a);
    UITextRenderer* SetTextColor(glm::vec4 color);
    UITextRenderer* SetRenderInScene(bool t);

public:
    Transform* tr;
    bool renderInScene;

    std::string text;
    glm::vec4 color;
    float textScale;

    bool renderTrigger;
};


#endif //ZERO_ENGINE_UITEXTRENDERER_H
