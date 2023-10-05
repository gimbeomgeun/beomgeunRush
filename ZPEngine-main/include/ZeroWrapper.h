//
// Created by EunwooSong on 2020-11-29.
//

#ifndef ZERO_ENGINE_ZEROWRAPPER_H
#define ZERO_ENGINE_ZEROWRAPPER_H

#include "ZeroMath.h"
#include <ft2build.h>
#include FT_FREETYPE_H

//////////////////// ZP_TEXTURE ////////////////////
class ZeroTexture {
public:
    ZeroTexture() = default;
    ~ZeroTexture();

    void BindTexture();

    void SetWidth(int width) { this->width = width; }
    void SetHeight(int height) { this->height = height; }
    void SetChannel(int channel) { this->channel = channel; }
    void SetWarpS(GLint warpS) {this->warpS = warpS;}
    void SetWarpT(GLint warpT) {this->warpT = warpT;}
    void SetFilterMin(GLint filterMin) {this->filterMin = filterMin;}
    void SetFilterMag(GLint filterMag) {this->filterMag = filterMag;}
    void SetTexture(GLuint texture) { this->texture = texture; }

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetChannel() const { return channel; }
    GLint GetWrapS() {return warpS;}
    GLint GetWrapT() {return warpT;}
    GLint GetFilterMin() {return filterMin;}
    GLint GetFilterMag() {return filterMag;}
    GLuint GetTexture() const { return texture; }

private:
    int width;
    int height;
    int channel;

    GLint warpS;
    GLint warpT;
    GLint filterMin;
    GLint filterMag;

    GLuint texture;
};


//////////////////// ZP_SHADER ////////////////////
class ZeroShader
{
public:
    ZeroShader() = default;
    ZeroShader(GLuint shaderID) : shaderID(shaderID) {}
    ~ZeroShader();

    // Sets the current shader as active
    ZeroShader& UseShader();

    // shader utility functions
    void    SetFloat    (const char *name, float value,                         bool useShader = false);
    void    SetInteger  (const char *name, int value,                           bool useShader = false);
    void    SetVector2f (const char *name, float x, float y,                    bool useShader = false);
    void    SetVector2f (const char *name, const glm::vec2 &value,              bool useShader = false);
    void    SetVector3f (const char *name, float x, float y, float z,           bool useShader = false);
    void    SetVector3f (const char *name, const glm::vec3 &value,              bool useShader = false);
    void    SetVector4f (const char *name, float x, float y, float z, float w,  bool useShader = false);
    void    SetVector4f (const char *name, const glm::vec4 &value,              bool useShader = false);
    void    SetMatrix4  (const char *name, const glm::mat4 &matrix,             bool useShader = false);

private:
    GLuint shaderID;
};

//////////////////// ZP_FONT ////////////////////
class ZeroFont {
public:
    ZeroFont() = default;
    ~ZeroFont() {
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

public:
    FT_Library ft;
    FT_Face face ;
};

//////////////////// ZP_GRAPHICS ////////////////////
struct Color {
    float r, g, b, a;

    Color() {
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;
        a = 1.0f;
    }
    Color(float r, float g, float b, float a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    Color(glm::vec4& color) {
        this->r = color.x;
        this->g = color.y;
        this->b = color.z;
        this->a = color.w;
    }
};

struct UV {
    float u; float v;
    UV() {
        u = 1.0f;
        v = 1.0f;
    }
    UV(float uv) {
        u = uv; v = uv;
    }
    UV(float u, float v) {
        this->u = u; this->v = v;
    }
};

struct Vertex {
    ZeroVec2 pos;
    Color color;
    UV uv;

    Vertex() {
        pos = Vec2(0, 0);
        color = Color(1,1,1,1);
        uv = UV(1.0f);
    }
    void SetPosition(float x, float y) { pos.x = x; pos.y = y; }
    void SetPosition(ZeroVec2 pos) { pos.x = pos.x; pos.y = pos.y; }
    void SetColor(float r, float g, float b, float a) { color = Color(r, g, b, a); }
    void SetUV(float u, float v) { uv.u = u, uv.v = v; }
    void SetUV(UV u) { uv = u; }
};

struct CharacterInfo {
    GLuint textureID;
    Vec2 size;
    Vec2 bearing;
    unsigned int Advance;
};

#endif //ZERO_ENGINE_ZEROWRAPPER_H
