//
// Created by EunwooSong on 2020-12-09.
//

#ifndef ZERO_ENGINE_SPRITE2DRENDERER_H
#define ZERO_ENGINE_SPRITE2DRENDERER_H

#include "ZeroComponent.h"
#include "ZeroMath.h"
#include "ZeroWrapper.h"
#include "Transform.h"

class Camera2D;
class ZeroRenderManager;

// -= Sprite2DRenderer =-
// - 화면에 이미지를 출력하는 컴포넌트
// - 이미 만들어진 쉐이더를 가져와 출력진행
class Sprite2DRenderer :
        public ZeroComponent {
public:
    Sprite2DRenderer() :
            texture(nullptr),
            color(1,1,1,1),
            width(0),
            height(0),
            isVisible(true),
            targetCamera(nullptr),
            sortLayer(0),
            orderLayer(0),
            renderTrigger(false),
            destRect(0,0,100,100),
            uvRect(0,0,1.0,1.0),
            flipX(false),
            flipY(false)
            { Init(); }
    ~Sprite2DRenderer() override;

    void Init();

    void Start() override;
    void Render() override;

    int GetWidth()                  { return width; }
    int GetHeight()                 { return height; }
    ZeroVec2 GetTextureSize()    { return Vec2(width, height); }
    glm::vec4 GetColor()            { return color; }
    ZeroTexture* GetTexture()    { return texture; }
    ZeroShader* GetShader()      { return shader; }
    int GetSortLayer()      { return this->sortLayer; }
    int GetOrderLayer()             { return orderLayer; }
    bool GetFlipX() { return flipX; }
    bool GetFlipY() { return flipY; }

    //스프라이트의 색을 설정하는 함수
    void SetColor(glm::vec4 color) { this->color = color; }
    void SetColor(float r, float g, float b, float a) {this->color = glm::vec4(r, g, b, a);}

    // 텍스쳐의 경로를 입력하여 스프라이트가 출력할 이미지를 설정하는 함수
    void SetTexture(std::string path);

    // 텍스쳐를 입력하여 스프라이트가 출력할 이미지를 설정하는 함수
    void SetTexture(ZeroTexture* texture);

    // 출력을 진행할 쉐이더의 이름 입력하여 설정하는 함수
    // 리소스 메니저에 로드된 쉐이더의 정보만 가져올 수 있다
    void SetShader(std::string shaderName);

    // 출력을 진행할 쉐이더의 이름, 경로를 입력하여 설정하는 함수
    // 리소스 메니저에서 해당 쉐이더를 로드하고 정보를 가져올 수 있다
    void SetShader(std::string shaderName, std::string vertex, std::string fragment, std::string geometry = "");
    void SetShader(ZeroShader* shader);

    // 스프라이트 렌더러의 레이어를 설정하는 함수
    // Layer는 해당 씬에 있는 렌더 메니저에서 미리 설정하고 입력해야한다.
    // 기본으로 0번째 레이어가 들어가 있다.
    void SetSortLayer(std::string layer);
    void SetSortLayer(int layer);

    // 설정된 레이어에서의 출력순서를 설정하는 함수
    void SetOrderLayer(int layer) {orderLayer = layer;}
    void StartRender() { renderTrigger = true;}

    // 텍스쳐를 X축으로 뒤집을지 여부를 설정하는 함수
    void SetFlipX(bool x) { flipX = x; }
    // 텍스쳐를 Y축으로 뒤집을지 여부를 설정하는 함수
    void SetFlipY(bool y) { flipY = y; }

    // VAO 설정
    void SetVAO(GLuint vao) { this->vao = vao; }
    void SetVBO(GLuint vbo) { this->vbo = vbo; }

protected:
    Camera2D* targetCamera;

    Vertex vertexInfo;

    GLuint vao;
    GLuint vbo;

    Transform* transform;
    ZeroShader* shader;
    ZeroTexture* texture;

    int sortLayer;
    int orderLayer;

    glm::vec4 color;
    glm::vec4 destRect;
    glm::vec4 uvRect;

    int width;
    int height;
    bool isVisible;
    bool renderTrigger;

    bool flipX;
    bool flipY;

    ZeroRenderManager* renderer;
};


#endif //ZERO_ENGINE_SPRITE2DRENDERER_H
