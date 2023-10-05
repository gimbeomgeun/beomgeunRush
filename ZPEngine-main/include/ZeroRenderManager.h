//
// Created by EunwooSong on 2020-12-15.
//

#ifndef ZERO_ENGINE_ZERORENDERMANAGER_H
#define ZERO_ENGINE_ZERORENDERMANAGER_H

#include "ZeroWrapper.h"
#include <map>
#include <vector>
#include <string>

#define BACKGROUND  0
#define OBJECT      1
#define UI          2

class ZeroWindow;
class ZeroSceneMgr;
class ZeroResourcesManager;
class LightCastBlock2D;
class PointLight2D;
class Sprite2DRenderer;
class ZeroShader;
class RigidBody2D;
class UITextRenderer;
class Camera2D;
class ZeroGameObject;

// ������ ������ �����ϴ� �޴��� �Դϴ�.
class ZeroRenderManager {
public:
    ZeroRenderManager(ZeroWindow* windowObj, ZeroResourcesManager* resourceObj);
    ~ZeroRenderManager();

    void Initialize();
    void Render();
    void EndScene();
    
    // ���� ����Ǹ� ���ε� �� ���� ���� ����� �ٲ� ���� ī�޶� ����
    void ChangeScene(ZeroGameObject* camObj);

    void Release();

    //Sprite2DRenderer�� ���� ť�� ����մϴ�.
    void AddSpriteRenderer      (Sprite2DRenderer* renderer);

    //2D Light�� ���� ť�� ����մϴ�. (������ ����� �� �����ϴ�)
    void AddLightCastBlock2D    (LightCastBlock2D* block);
    void AddPointLight          (PointLight2D* light);

    //������ RigidBody2D�� ���� ť�� ����մϴ�. (�׵θ� ���)
    void AddRigidBody2D         (RigidBody2D* rigid);

    //UITextRenderer�� ���� ť�� ����մϴ�.
    void AddTextRenderer        (UITextRenderer* text);

    void RenderQuad();
    void RenderText(std::string text, Vec2 modelPos, glm::vec4& color, float scale, bool renderInScene);
    void AddText(std::string text, int len = -1);

    void InitASCIIText(std::string fontpath = "font/DOSMyungjo.ttf");

    void AddLayerInfo(int i, std::string s);
    bool CheckLayerInfo(int i);
    bool CheckLayerInfo(std::string s);
    std::string GetLayerInfo(int i);
    int GetLayerInfo(std::string s);

private:
    Camera2D* targetCam;

    //Layer(sort, order) Information
    std::map<int, std::string> layerInfo;

    //Sprite RenderQueue
    typedef std::vector<Sprite2DRenderer*> RenderQueue;
    typedef std::map<int, RenderQueue> RendererLayer;

    //Light RenderQueue
    typedef std::vector<LightCastBlock2D*> LightCastBlockQueue;
    typedef std::vector<PointLight2D*> LightQueue;

    //Box RenderQueue
    typedef std::vector<RigidBody2D*> RigidQueue;

    //Text RenderQueue
    typedef std::vector<UITextRenderer*> TextQueue;

    RendererLayer           renderLayers;
    LightCastBlockQueue     blockQueue;
    LightQueue              lightQueue;
    RigidQueue              rigidQueue;
    TextQueue               textQueue;

    //For PostProcessing
    ZeroShader* hdrShader;

    // HDR Settings
    bool hdr = true;
    float exposure = 1.0f;

    //FrameBuffers
    GLuint hdrFBO;
    GLuint colorBuffer[2];

    //For Test
    float shadowLength;

    //Quad 1x1
    GLuint quadVAO;
    GLuint quadVBO;

    //For Font
    ZeroShader* fontShader;
    GLuint fontVAO;
    GLuint fontVBO;
    std::map<int, CharacterInfo> Characters;
    ZeroFont* font;

    ZeroWindow* windowObj;
    ZeroSceneMgr* sceneObj;
    ZeroResourcesManager* resourceObj;
};


#endif //ZERO_ENGINE_ZERORENDERMANAGER_H
