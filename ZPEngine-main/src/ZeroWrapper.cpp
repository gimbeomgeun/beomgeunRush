//
// Created by EunwooSong on 2020-11-29.
//

#include "stdafx.h"
#include "ZeroWrapper.h"

//////////////////// ZP_TEXTURE ////////////////////
void ZeroTexture::BindTexture() {
    //Bind the texture as the current activie GL_TEXTURE_2D texture object
    glBindTexture(GL_TEXTURE_2D, texture);
}

ZeroTexture::~ZeroTexture() {
    if(glIsTexture(texture) == GL_TRUE)
        glDeleteTextures(1, &texture);
}

//////////////////// ZP_SHADER ////////////////////
ZeroShader &ZeroShader::UseShader() {
    glUseProgram(shaderID);
    return *this;
}

void ZeroShader::SetFloat(const char *name, float value, bool useShader) {
    if (useShader) this->UseShader();
    glUniform1f(glGetUniformLocation(shaderID, name), value);
}

void ZeroShader::SetInteger(const char *name, int value, bool useShader) {
    if (useShader) this->UseShader();
    glUniform1i(glGetUniformLocation(shaderID, name), value);
}

void ZeroShader::SetVector2f(const char *name, float x, float y, bool useShader) {
    if (useShader) this->UseShader();
    glUniform2f(glGetUniformLocation(shaderID, name), x, y);
}

void ZeroShader::SetVector2f(const char *name, const glm::vec2 &value, bool useShader) {
    if (useShader) this->UseShader();
    glUniform2f(glGetUniformLocation(shaderID, name), value.x, value.y);
}

void ZeroShader::SetVector3f(const char *name, float x, float y, float z, bool useShader) {
    if (useShader) this->UseShader();
    glUniform3f(glGetUniformLocation(shaderID, name), x, y, z);
}

void ZeroShader::SetVector3f(const char *name, const glm::vec3 &value, bool useShader) {
    if (useShader) this->UseShader();
    glUniform3f(glGetUniformLocation(shaderID, name), value.x, value.y, value.z);
}

void ZeroShader::SetVector4f(const char *name, float x, float y, float z, float w, bool useShader) {
    if (useShader) this->UseShader();
    glUniform4f(glGetUniformLocation(shaderID, name), x, y, z, w);
}

void ZeroShader::SetVector4f(const char *name, const glm::vec4 &value, bool useShader) {
    if (useShader) this->UseShader();
    glUniform4f(glGetUniformLocation(shaderID, name), value.x, value.y, value.z, value.w);
}

void ZeroShader::SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader) {
    if (useShader) this->UseShader();
    glUniformMatrix4fv(glGetUniformLocation(this->shaderID, name), 1, false, glm::value_ptr(matrix));
}

ZeroShader::~ZeroShader() {
    glDeleteProgram(shaderID);
}
