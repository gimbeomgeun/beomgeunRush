//
// Created by EunwooSong on 2020-12-15.
//

#include "stdafx.h"
#include "ZeroSystem.h"
#include "ZeroRenderManager.h"
#include "Sprite2DRenderer.h"
#include "Transform.h"
#include "ZeroGameObject.h"
#include "ZeroWindow.h"
#include "ZeroLogger.h"
#include "ZeroInputMgr.h"
#include "ZeroSceneMgr.h"
#include "RigidBody2D.h"
#include "UITextRenderer.h"
#include "ZeroLogger.h"
#include "Camera2D.h"


ZeroRenderManager::ZeroRenderManager(ZeroWindow* windowObj, ZeroResourcesManager* resourceObj)
    : shadowLength((float)windowObj->GetWidth()), quadVAO(0),
    hdr(false), exposure(1.0f), targetCam(nullptr),
    windowObj(windowObj), resourceObj(resourceObj) {
}

ZeroRenderManager::~ZeroRenderManager() {

}

void ZeroRenderManager::Initialize() {
//Initialize LayerInfo
    AddLayerInfo(BACKGROUND,    "BACKGROUND");
    AddLayerInfo(OBJECT,        "OBJECT");
    AddLayerInfo(UI,            "UI");

//Physics
    int width = windowObj->GetWidth();
    int height = windowObj->GetHeight();

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // configure VAO/VBO
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

// Font
    InitASCIIText();
}

void ZeroRenderManager::Render() {
    //Clear Buffer
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //For Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for(auto layer : layerInfo) {

        for (auto iter : renderLayers[layer.first]) {
            if (iter->GetIsStarted())
                if(iter->GetActive()) {
                    iter->StartRender();
                    iter->Render();
                }
        }
    }

    for(auto iter : textQueue) {
        if(iter->GetIsStarted()) {
            if(iter->GetActive()) {
                iter->StartRender();
                iter->Render();
            }
        }
    }

#if defined(DEBUG) | defined(_DEBUG)
    for(RigidBody2D* iter : rigidQueue) {
        if(iter->GetIsStarted()) {
            if(iter->GetActive()) {
                iter->StartRender();
                iter->Render();
            }
        }
    }
#endif

    //Blending 종료
    glDisable(GL_BLEND);
}

void ZeroRenderManager::EndScene() {
    for(RendererLayer::iterator iter = renderLayers.begin(); iter != renderLayers.end(); iter++) {
        iter->second.erase(remove_if(iter->second.begin(), iter->second.end(),
                                     [&](Sprite2DRenderer* renderer) { return renderer->GetOwner()->GetIsDestroy(); }),
                           iter->second.end());
    }

    rigidQueue.erase(remove_if(rigidQueue.begin(), rigidQueue.end(),
                          [&](RigidBody2D* renderer) { return renderer->GetOwner()->GetIsDestroy(); }),
                     rigidQueue.end());

    textQueue.erase(remove_if(textQueue.begin(), textQueue.end(),
                               [&](UITextRenderer* renderer) { return renderer->GetOwner()->GetIsDestroy(); }),
                    textQueue.end());
}

void ZeroRenderManager::ChangeScene(ZeroGameObject* camObj)
{
    renderLayers.clear();
    blockQueue.clear();
    lightQueue.clear();
    rigidQueue.clear();
    textQueue.clear();

    // 변경된 씬에서의 카메라 찾기
    targetCam = camObj->GetComponent<Camera2D>();
    float aspect = float(windowObj->GetWidth()) / float(windowObj->GetHeight());
    glOrtho(-targetCam->scale * aspect, targetCam->scale * aspect, -targetCam->scale + targetCam->pan_y, targetCam->scale + targetCam->pan_y, -1.0, 1.0);
}

void ZeroRenderManager::AddSpriteRenderer(Sprite2DRenderer* renderer) {
    if(!CheckLayerInfo(renderer->GetSortLayer())) {
        ZERO_LOGGER::Error("Render Manager : SortLayer(ID : %d) is not initialized. Please check Sprite2DRenderer's sortLayerID. ObjectID : %d", renderer->GetSortLayer(), renderer->GetOwner()->GetEntityID());
    }

    renderLayers[renderer->GetSortLayer()].push_back(renderer);

    //Sort RenderQueue
    std::sort(renderLayers[renderer->GetSortLayer()].begin(),
              renderLayers[renderer->GetSortLayer()].end(),
              [&](Sprite2DRenderer *a, Sprite2DRenderer *b) {
        return a->GetOrderLayer() < b->GetOrderLayer();
    });

    // Set VAO/VBO
    renderer->SetVAO(quadVAO);
    renderer->SetVBO(quadVBO);
}

void ZeroRenderManager::AddLightCastBlock2D(LightCastBlock2D *block) {
    blockQueue.push_back(block);
}

void ZeroRenderManager::AddPointLight(PointLight2D *light) {
    lightQueue.push_back(light);
}

void ZeroRenderManager::RenderQuad() {
//    glBindVertexArray(quadVAO);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
//    glBindVertexArray(0);
}

void ZeroRenderManager::AddRigidBody2D(RigidBody2D *rigid) {
    rigidQueue.push_back(rigid);

    rigid->SetVAO(quadVAO);
    rigid->SetVAO(quadVBO);
}

void ZeroRenderManager::AddTextRenderer(UITextRenderer *text) {
    textQueue.push_back(text);
}

void ZeroRenderManager::RenderText(std::string text, Vec2 modelPos, glm::vec4& color, float scale, bool renderInScene) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fontShader->UseShader();
    fontShader->SetVector4f("textColor", color);
    if(renderInScene) {
        fontShader->SetMatrix4("projection", targetCam->GetProjectionMatrix());
        fontShader->SetMatrix4("view", targetCam->view);
    }
    else {
        fontShader->SetMatrix4("projection", targetCam->uiProjection);
        glm::mat4 view = ZeroMath::MatrixTransformation(Vec2(0.0f, 0.0f), Vec2(1.0f, 1.0f), 0, Vec2(0.0f, 0.0f));
        fontShader->SetMatrix4("view", view);

        scale *= -1;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(fontVAO);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        CharacterInfo ch = Characters[*c];

        float xPos = modelPos.x + ch.bearing.x * std::fabsf(scale);
        float yPos = modelPos.y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * std::fabsf(scale);
        float h = ch.size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
                {xPos,     yPos + h, 0.0f, 0.0f },
                {xPos,     yPos,     0.0f, 1.0f },
                {xPos + w, yPos,     1.0f, 1.0f },

                {xPos,     yPos + h, 0.0f, 0.0f },
                {xPos + w, yPos,     1.0f, 1.0f },
                {xPos + w, yPos + h, 1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        modelPos.x += (ch.Advance >> 6) * std::fabsf(scale); // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
}

void ZeroRenderManager::AddText(std::string text, int len) {
    if(font) {
        if(len == -1) {
            len = 0;

            for(int i = 0; text[i]; i++) {
                if(text[i] & 0x80) i++;
                len++;
            }

            len = text.size();
        }

        for (unsigned char c = 0; c < len; c++) {
            if(text[c] < 0x80) continue; //한글만 추가

            int target = text[c++] << 8; target |= text[c];

            // Load character glyph
            if (FT_Load_Char(font->face, target, FT_LOAD_RENDER)) {
                std::cout << "font/FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    font->face->glyph->bitmap.width,
                    font->face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    font->face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            CharacterInfo character = {
                    texture,
                    Vec2(font->face->glyph->bitmap.width, font->face->glyph->bitmap.rows),
                    Vec2(font->face->glyph->bitmap_left, font->face->glyph->bitmap_top),
                    static_cast<unsigned int>(font->face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, CharacterInfo>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void ZeroRenderManager::InitASCIIText(std::string fontpath) {
    font = resourceObj->LoadFont("font/NanumGothic.ttf");
    fontShader = resourceObj->LoadShaders("font", "shaders/TextShader/TextShader.vert", "shaders/TextShader/TextShader.frag");
    if (font) {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(font->face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
           

            // Load character glyph
            if (FT_Load_Char(font->face, c, FT_LOAD_RENDER))
            {
                std::cout << "font/FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                font->face->glyph->bitmap.width,
                font->face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                font->face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            CharacterInfo character = {
                    texture,
                    Vec2(font->face->glyph->bitmap.width, font->face->glyph->bitmap.rows),
                    Vec2(font->face->glyph->bitmap_left, font->face->glyph->bitmap_top),
                    static_cast<unsigned int>(font->face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, CharacterInfo>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        // configure VAO/VBO for texture quads
        // -----------------------------------
        glGenVertexArrays(1, &fontVAO);
        glGenBuffers(1, &fontVBO);
        glBindVertexArray(fontVAO);
        glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

//Layer
void ZeroRenderManager::AddLayerInfo(int i, std::string s) {
    if(!CheckLayerInfo(i)) {
        if(!CheckLayerInfo(s)) {
            layerInfo.insert(std::pair<int, std::string>(i, s));
            return;
        }
    }

    ZERO_LOGGER::Error("RenderManager : Duplicated Layer ID!  %d / %s", i, s.c_str());
}

bool ZeroRenderManager::CheckLayerInfo(int i) {
    if(layerInfo.empty())
        return false;

    return layerInfo.count(i);
}

bool ZeroRenderManager::CheckLayerInfo(std::string s) {
    if(layerInfo.empty())
        return false;

    for(auto iter = layerInfo.begin(); iter != layerInfo.end(); iter++) {
        if(s.compare(iter->second) == 0) {
            return true;
        }
    }

    return  false;
}

std::string ZeroRenderManager::GetLayerInfo(int i) {
    if(CheckLayerInfo(i))
        return layerInfo[i];

    ZERO_LOGGER::Error("RenderManager : Cannot Find Layer name! ID : %d", i);
    return "";
}

int ZeroRenderManager::GetLayerInfo(std::string s) {
    for(auto iter = layerInfo.begin(); iter != layerInfo.end(); iter++) {
        if(s.compare(iter->second) == 0) {
            return iter->first;
        }
    }

    ZERO_LOGGER::Error("RenderManager : Cannot Find Layer ID! name : %s", s.c_str());
    return NULL;
}

void ZeroRenderManager::Release() {
    glDeleteVertexArrays(1, &this->quadVAO);
    glDeleteBuffers(1, &this->quadVBO);

    glDeleteVertexArrays(1, &this->fontVAO);
    glDeleteBuffers(1, &this->fontVBO);

    for (auto iter = Characters.begin(); iter != Characters.end(); iter++) {
        glDeleteTextures(1, &iter->second.textureID);
    }

    renderLayers.clear();
    blockQueue.clear();
    lightQueue.clear();
    rigidQueue.clear();
    textQueue.clear();
    Characters.clear();
}
