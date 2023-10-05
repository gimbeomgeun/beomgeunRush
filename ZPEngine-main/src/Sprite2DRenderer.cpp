//
// Created by EunwooSong on 2020-12-09.
//

#include "stdafx.h"
#include "ZeroGameObject.h"
#include "Sprite2DRenderer.h"
#include "ZeroWindow.h"
#include "ZeroSceneMgr.h"
#include "Camera2D.h"
#include "ZeroSystem.h"

Sprite2DRenderer::~Sprite2DRenderer() {

}

void Sprite2DRenderer::Init() {
    renderer = ZERO_SCENE_MGR->GetCurrentScene()->GetRenderManager();

    SetShader("Texture Shader", "shaders/TextureShader/TextureShader.vert", "shaders/TextureShader/TextureShader.frag");
}

void Sprite2DRenderer::Start() {
    ZeroComponent::Start();

    transform = GetOwner()->GetComponent<Transform>();

    //For Test
    if(shader)
        shader->UseShader().SetInteger("image", 0);

    //Find Camera2D
    if(!targetCamera)
        targetCamera = ZERO_SCENE_MGR->GetCurrentScene()->FindGameObjectWithTag("MainCamera")->GetComponent<Camera2D>();

    //Add RenderQueue
    ZERO_SCENE_MGR->GetCurrentScene()->GetRenderManager()->AddSpriteRenderer(this);
}

void Sprite2DRenderer::Render() {
    //Render Trigger;
    if(!renderTrigger) return;

    ZeroComponent::Render();

    if(!shader || !texture || !targetCamera) return;
    //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

//    glm::mat4 scale(1);
//    scale = glm::scale(scale, glm::vec3(width, height, 0.0f));

    //이미지의 중심을 설정합니다
    Vec2 center = Vec2(width * (flipX ? -1.0f : 1.0f) * transform->GetScale().x, -height * (flipY ? -1.0f : 1.0f) * transform->GetScale().y);
    Vec2 scaleCenter = transform->GetScaleCenter();
    Vec2 imagePos; //center * scaleCenter;
    imagePos.x = (center * scaleCenter).x;
    imagePos.y = (center * scaleCenter).y;

    //모델 메트릭스를 생성합니다.
    glm::mat4 model = ZeroMath::MatrixTransformation(transform->GetScaleCenter(), center, transform->GetWorldRotation(), transform->GetWorldPos() - imagePos);

    shader->UseShader();
    this->shader->SetMatrix4("projection", targetCamera->projection);
    this->shader->SetMatrix4("view", targetCamera->view);
    this->shader->SetMatrix4("model", model);
    this->shader->SetVector4f("spriteColor", color);

    //텍스쳐를 바인딩합니다.
    glActiveTexture(GL_TEXTURE0);
    texture->BindTexture();

    //VAO를 바인딩합니다.
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    renderTrigger = false;
}

// - = Set Texture = -------------------------------------------------------
void Sprite2DRenderer::SetTexture(ZeroTexture *texture) {
    this->texture = texture;
    width = texture->GetWidth();
    height = texture->GetHeight();
}

void Sprite2DRenderer::SetTexture(std::string path) {
    this->texture = ZERO_RESOURCES_MGR->LoadTexture(path);
    width = texture->GetWidth();
    height = texture->GetHeight();
}

//void Sprite2DRenderer::SetTexture(const char *path) {
//    this->texture = ZERO_RESOURCES_MGR->LoadTexture(std::string(path));
//    width = texture->GetWidth();
//    height = texture->GetHeight();
//}

// - = Set Shader = -------------------------------------------------------
void Sprite2DRenderer::SetShader(std::string shaderName) {
    this->shader = ZERO_RESOURCES_MGR->GetShader(shaderName);
}

void Sprite2DRenderer::SetShader( std::string shaderName, std::string vertex, std::string fragment, std::string geometry) {
    this->shader = ZERO_RESOURCES_MGR->LoadShaders(shaderName, vertex, fragment, geometry);
}

void Sprite2DRenderer::SetShader(ZeroShader *shader) {
    this->shader = shader;
}

void Sprite2DRenderer::SetSortLayer(std::string layer) {
    sortLayer = this->GetOwner()->GetScene()->GetRenderManager()->GetLayerInfo(layer);
}

void Sprite2DRenderer::SetSortLayer(int layer) {
    if(this->GetOwner()->GetScene()->GetRenderManager()->CheckLayerInfo(layer))
        sortLayer = layer;
}
