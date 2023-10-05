//
// Created by EunwooSong on 2020-12-27.
//

#include "stdafx.h"
#include "ZeroSystem.h"
#include "UITextRenderer.h"
#include "ZeroScene.h"
#include "ZeroGameObject.h"

void UITextRenderer::Render() {
    ZeroComponent::Render();
    if(!renderTrigger) return;

    ZERO_SCENE_MGR->GetCurrentScene()->GetRenderManager()->RenderText(text, tr->GetWorldPos(), color, textScale, renderInScene);

    renderTrigger = false;
}

void UITextRenderer::Start() {
    ZeroComponent::Start();
    tr = GetOwner()->transform;
    ZERO_SCENE_MGR->GetCurrentScene()->GetRenderManager()->AddTextRenderer(this);
}

UITextRenderer *UITextRenderer::SetText(std::string text, int len) {
    this->text = text;
    ZERO_SCENE_MGR->GetCurrentScene()->GetRenderManager()->AddText(text, len);
    return this;
}

UITextRenderer *UITextRenderer::SetTextScale(float scale) {
    textScale = scale;
    return this;
}

UITextRenderer *UITextRenderer::SetTextColor(float r, float g, float b, float a) {
    color.x = r;
    color.y = g;
    color.z = b;
    color.w = a;
    return this;
}

UITextRenderer *UITextRenderer::SetTextColor(glm::vec4 color) {
    this->color = color;
    return this;
}

UITextRenderer *UITextRenderer::SetRenderInScene(bool t) {
    renderInScene = t;
    return this;
}