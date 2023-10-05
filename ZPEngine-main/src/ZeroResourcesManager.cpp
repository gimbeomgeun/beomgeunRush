//
// Created by EunwooSong on 2020-11-29.
//

#include "stdafx.h"
#include "ZeroResourcesManager.h"
#include "ZeroWrapper.h"
#include "ZeroLogger.h"
#include "ZeroSoundDevice.h"
#include "ZeroSystem.h"

#include <sndfile.h>
#include <inttypes.h>
#include <AL\alext.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

ZeroResourcesManager::ZeroResourcesManager() : basePath(nullptr), fonts(nullptr), shaders(nullptr), sounds(nullptr), textures(nullptr) {

}

ZeroResourcesManager::~ZeroResourcesManager() {
    //Release();
}

void ZeroResourcesManager::Initialize() {
    fonts = new std::map<std::string, ZeroFont*>();
    textures = new std::map<std::string, ZeroTexture*>();
    shaders = new std::map<std::string, ZeroShader*>();
    sounds = new std::vector<ALuint>();

    basePath = new std::string("../Resources/");
}

ZeroTexture *ZeroResourcesManager::LoadTexture(std::string filename, bool isAlpha, GLint wrapS, GLint wrapT, GLint filterMin, GLint filterMag) {
    ZeroTexture* iter = nullptr;
    
    if ((*textures).find(filename) != (*textures).end())
        iter = (*textures)[filename];

    if (!iter) {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        std::string tmpPath = (*basePath) + filename;

        int width, height, nrChannels;
        unsigned char* data = stbi_load(tmpPath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            if (isAlpha)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            //Create mipmaps
            //glGenerateMipmap(GL_TEXTURE_2D);

            //GLenum pname, const GLint *params
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);

            glBindTexture(GL_TEXTURE_2D, 0);
        }
        else
        {
            ZERO_LOGGER::Error("Failed to load image : %s", tmpPath.c_str());
        }
        stbi_image_free(data);

        ZeroTexture* tmp = new ZeroTexture();
        tmp->SetTexture(texture);
        tmp->SetWidth(width);
        tmp->SetHeight(height);
        tmp->SetChannel(nrChannels);
        tmp->SetWarpS(wrapS);
        tmp->SetWarpT(wrapT);
        tmp->SetFilterMin(filterMin);
        tmp->SetFilterMag(filterMag);

        (*textures)[filename] = tmp;
        iter = tmp;
    }

    return iter;
}

ZeroShader *ZeroResourcesManager::LoadShaders(std::string name, std::string vertex, std::string fragment, std::string geometry) {
    if(GetShader(name) != nullptr)
        return GetShader(name);

    vertex.insert(0, (*basePath).c_str());
    fragment.insert(0, (*basePath).c_str());

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint GeometryShaderID = NULL;

    std::string VertexShaderCode;
    std::ifstream VertexShaderStream;
    VertexShaderStream.open(vertex, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }else{
        ZeroLogger::Error("%s \n", vertex.c_str());
        return 0;
    }

    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream;
    FragmentShaderStream.open(fragment, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();

        FragmentShaderStream.close();
    }
    else {
        ZeroLogger::Error("%s \n", fragment.c_str());
        return 0;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex shaders
    ZERO_LOGGER::Info("Compiling shader : %s", vertex.c_str());
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex shaders
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        ZeroLogger::Error("%s\n", &VertexShaderErrorMessage[0]);
    }



    // Compile Fragment shaders
    ZERO_LOGGER::Info("Compiling shader : %s", fragment.c_str());
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment shaders
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        ZeroLogger::Error("%s\n", &FragmentShaderErrorMessage[0]);
    }

    if(!geometry.empty()) {
        geometry.insert(0, (*basePath).c_str());

        GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

        std::string GeometryShaderCode;
        std::ifstream GeometryShaderStream;
        GeometryShaderStream.open(geometry, std::ios::in);
        if(GeometryShaderStream.is_open()){
            std::stringstream sstr;
            sstr << GeometryShaderStream.rdbuf();
            GeometryShaderCode = sstr.str();
            GeometryShaderStream.close();
        }else{
            ZeroLogger::Error("%s \n", geometry.c_str());
            return 0;
        }

        // Compile Geometry shaders
        ZERO_LOGGER::Info("Compiling shader : %s", geometry.c_str());
        char const * GeometrySourcePointer = GeometryShaderCode.c_str();
        glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer , NULL);
        glCompileShader(GeometryShaderID);

        // Check Geometry shaders
        glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> GeometryShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL, &GeometryShaderErrorMessage[0]);
            ZeroLogger::Error("%s\n", &GeometryShaderErrorMessage[0]);
        }
    }

    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    if(!geometry.empty())
        glAttachShader(ProgramID, GeometryShaderID);

    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        ZeroLogger::Error("%s", &ProgramErrorMessage[0]);
    }

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);
    if(!geometry.empty())
        glDetachShader(ProgramID, GeometryShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    if(!geometry.empty())
        glDeleteShader(GeometryShaderID);

    ZeroShader* tmpShader = new ZeroShader(ProgramID);

    (*shaders)[name] = tmpShader;

    return tmpShader;
}

ZeroShader *ZeroResourcesManager::GetShader(std::string name) {
    if((*shaders)[name]) return (*shaders)[name];
    else {
        ZERO_LOGGER::Error("shader[%s] is nullptr", name.c_str());
        return nullptr;
    }
}

void ZeroResourcesManager::Release() {
    for(auto iter : *textures) {
        SAFE_DELETE(iter.second);
    }

    for(auto iter : *shaders) {
        SAFE_DELETE(iter.second);
    }

    for(auto iter : *fonts) {
        SAFE_DELETE(iter.second);
    }

    alDeleteBuffers((ALsizei)(*sounds).size(), (*sounds).data());

    SAFE_DELETE(fonts);
    SAFE_DELETE(textures);
    SAFE_DELETE(shaders);
    SAFE_DELETE(sounds);
    SAFE_DELETE(basePath);
}

ZeroFont *ZeroResourcesManager::LoadFont(std::string name) {
    if((*fonts)[name]) return (*fonts)[name];

    name.insert(0, (*basePath).c_str());

    ZeroFont* font = new ZeroFont();
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&(font->ft)))
    {
        ZERO_LOGGER::Error("FREETYPE error: Could not init FreeType Library");
        SAFE_DELETE(font);

        return nullptr;
    }

    // find path to font
    if (name.empty())
    {
        ZERO_LOGGER::Error("FREETYPE error: Failed to load font_name");
        SAFE_DELETE(font);

        return nullptr;
    }

    // load font as face
    if (FT_New_Face(font->ft, name.c_str(), 0, &(font->face))) {
        ZERO_LOGGER::Error("FREETYPE error: Failed to load font");
        SAFE_DELETE(font);

        return nullptr;
    }

    (*fonts)[name] = font;
    return font;
}

ALuint ZeroResourcesManager::LoadAudio(std::string path) {
    ALenum err, format;
    ALuint buffer;
    SNDFILE* sndFile;
    SF_INFO sfInfo;
    short* memBuf;
    sf_count_t num_frames;
    ALsizei num_bytes;

    path.insert(0, (*basePath).c_str());
    const char* p = path.c_str();

    // Open the audio file and check
    sndFile = sf_open(p, SFM_READ, &sfInfo);
    if (!sndFile)
    {
        ZERO_LOGGER::Error("Could not open audio in %s: %s\n", p, sf_strerror(sndFile));
        return 0;
    }
    if (sfInfo.frames <1 || sfInfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfInfo.channels)
    {
        ZERO_LOGGER::Error("Bad sample count in %s (%" PRId64 ")\n", p, sfInfo.frames);
        sf_close(sndFile);
        return 0;
    }

    // Get the sound format, and figure out the OpenAL format
    format = AL_NONE;
    if (sfInfo.channels == 1)
        format = AL_FORMAT_MONO16;
    else if (sfInfo.channels == 2)
        format = AL_FORMAT_STEREO16;
    else if (sfInfo.channels == 3)
    {
        if (sf_command(sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            format = AL_FORMAT_BFORMAT2D_16;
    }
    else if (sfInfo.channels == 4)
    {
        if (sf_command(sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            format = AL_FORMAT_BFORMAT3D_16;
    }
    if (!format)
    {
        ZERO_LOGGER::Error("Unsupported channel count: %d\n", sfInfo.channels);
        sf_close(sndFile);
        return 0;
    }

    // Decode the whole audio file to a buffer.
    memBuf = static_cast<short*>(malloc((size_t)(sfInfo.frames * sfInfo.channels) * sizeof(short)));

    num_frames = sf_readf_short(sndFile, memBuf, sfInfo.frames);
    if (num_frames < 1)
    {
        free(memBuf);
        sf_close(sndFile);
        ZERO_LOGGER::Error("Failed to read samples in %s (%" PRId64 ")\n", path, num_frames);
        return 0;
    }
    num_bytes = (ALsizei)(num_frames * sfInfo.channels) * (ALsizei)sizeof(short);

    buffer = 0;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, memBuf, num_bytes, sfInfo.samplerate);

    free(memBuf);
    sf_close(sndFile);

    // Check if an error occured, and clean up
    err = alGetError();
    if (err != AL_NO_ERROR)
    {
        ZERO_LOGGER::Error("OpenAL Error: %s\n", alGetString(err));
        if (buffer && alIsBuffer(buffer))
            alDeleteBuffers(1, &buffer);
        return 0;
    }

    (*sounds).push_back(buffer);  // add to the list of known buffers

    return buffer;
}

bool ZeroResourcesManager::UnLoadAudio(const ALuint &id) {
    bool result = false;

    (*sounds).erase(remove_if((*sounds).begin(), (*sounds).end(),
                           [&](ALuint bufferID) {
        if(bufferID == id) {
            alDeleteBuffers(1, &bufferID);
            result = true;
        }

        return bufferID == id;
    }), (*sounds).end());

    return result;  // couldn't find to remove
}