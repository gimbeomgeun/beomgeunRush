//
// Created by EunwooSong on 2020-12-24.
//

#include "stdafx.h"
#include "ZeroSystem.h"
#include "UIRenderer.h"
#include "ZeroGameObject.h"
#include "Sprite2DRenderer.h"
#include "ZeroWindow.h"
#include "Camera2D.h"
#include "RectTransform.h"

UIRenderer::UIRenderer() : isRayTarget(true) {
    
}

void UIRenderer::Start()
{
    Component::Start();

    SetSortLayer("UI");

    rectTransform = GetOwner()->GetComponent<RectTransform>();

    //For Test
    if (shader)
        shader->UseShader().SetInteger("image", 0);

    //Find Camera2D
    if (!targetCamera)
        targetCamera = ZERO_SCENE_MGR->GetCurrentScene()->FindGameObjectWithTag("MainCamera")->GetComponent<Camera2D>();

    //Add RenderQueue
    ZERO_SCENE_MGR->GetCurrentScene()->GetRenderManager()->AddSpriteRenderer(this);

    //Add EventManager
    ZERO_SCENE_MGR->GetCurrentScene()->GetEventManager()->AddTargetObject(GetOwner());

}

void UIRenderer::Render() {
    Component::Render();

    //Render Trigger;
    if(!renderTrigger) return;

    ZeroComponent::Render();

    if(!shader || !texture || !targetCamera) return;
    //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

//    glm::mat4 scale(1);
//    scale = glm::scale(scale, glm::vec3(width, height, 0.0f));



    Vec2 center = rectTransform->GetMySize(); //그릴 범위
    Vec2 scaleCenter = rectTransform->GetPivot();

    glm::mat4 model =
            ZeroMath::MatrixTransformation(rectTransform->GetScaleCenter(),
                                           Vec2(center.x * rectTransform->GetScale().x, center.y * rectTransform->GetScale().y),
                rectTransform->GetWorldRotation(), rectTransform->GetWorldPos());
    glm::mat4 view = ZeroMath::MatrixTransformation(Vec2(0.0f, 0.0f), Vec2(1.0f, 1.0f), 0, Vec2(0.0f, 0.0f));

    shader->UseShader();
    this->shader->SetMatrix4("projection", targetCamera->uiProjection );
    this->shader->SetMatrix4("view",    view);
    this->shader->SetMatrix4("model",   model);
    this->shader->SetVector4f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture->BindTexture();

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    renderTrigger = false;
}

void UIRenderer::SetNagativeSize()
{
    if (texture) {
        Vec2 textureSize;
        textureSize.x = texture->GetWidth();
        textureSize.y = texture->GetHeight();

        Vec2 p_rect = rectTransform->GetParentSize() * (rectTransform->GetAnchorMax() - rectTransform->GetAnchorMin());

        rectTransform->SetSizeDelta(textureSize - p_rect);
    }
}