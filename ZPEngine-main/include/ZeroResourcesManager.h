//
// Created by EunwooSong on 2020-11-29.
//

#ifndef ZERO_ENGINE_ZERORESOURCESMANAGER_H
#define ZERO_ENGINE_ZERORESOURCESMANAGER_H

#pragma warning( disable : 4819)

#include <string>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <AL/al.h>

class ZeroTexture;
class ZeroShader;
class ZeroFont;

class ZeroResourcesManager {
public:
    ZeroResourcesManager();
    ~ZeroResourcesManager();

    void Initialize();

    //-=Load Texture=-
    // * 텍스쳐 로더
    //  - ../Resources/ 가 맨 앞에 자동으로 붙어서 경로를 생성한다.
    //
    // wrapS, wrapT - 범위 밖의 좌표를 그릴 때의 처리 방법
    //  ├ GL_REPEAT: 텍스처의 기본 동작입니다. 이미지를 반복합니다.
    //  ├ GL_MIRRORED_REPEAT: GL_REPEAT와 같지만 반복할때마다 이미지를 반대로 뒤집습니다.
    //  ├ GL_CLAMP_TO_EDGE: 0과 1 사이의 좌표를 고정합니다. 결과적으로 큰 좌표가 가장자리에 고정되어 가장자리의 패턴이 늘어나게 됩니다.
    //  └ GL_CLAMP_TO_BORDER: 범위 밖의 좌표에 사용자가 지정한 테두리 색이 지정됩니다.
    //---------------------------------------------------------------------------------------
    //
    // filterMin, filterMag - 텍스쳐를 확대, 축소할 때의 처리 방법
    //  ├ GL_NEAREST: 픽셀들을 명확히 볼 수 있는 차단된 패턴을 생성(8비트)
    //  └ GL_LINEAR: 별 픽셀들이 덜 보이는 더 매끄러운 패턴을 생성
    //
    // + 텍스쳐를 축소할 때의 Mipmap 처리 방법(단, 텍스쳐가 확대될 때의 Mipmap은 생성되지 않는다 -> 오류 생성)
    //  ├ GL_NEAREST_MIPMAP_NEAREST : nearest neighbor 보간법으로 mipmap을 필터링하고 텍스처 샘플링도 nearest neghbor 보간법을 사용합니다.
    //  ├ GL_LINEAR_MIPMAP_NEAREST  : nearest neighbor 보간법으로 mipmap을 필터링하고 텍스처 샘플링은 linear 보간법을 사용합니다.
    //  ├ GL_NEAREST_MIPMAP_LINEAR  : linear 보간법으로 mipmap을 필터링하고 텍스처 샘플링은 nearest neighbor 보간법을 사용합니다.
    //  └ GL_LINEAR_MIPMAP_LINEAR   : linear 보간법으로 mipmap을 필터링하고 텍스처 샘플링도 linear 보간법을 사용합니다.
    ZeroTexture* LoadTexture(std::string filename,
                             bool isAlpha = true,
                             GLint wrapS = GL_REPEAT,
                             GLint wrapT = GL_REPEAT,
                             GLint filterMin = GL_NEAREST,
                             GLint filterMag = GL_NEAREST);

    //-=Load Shaders=-
    // * 쉐이더 로더 (Vertex와 Fragment의 주소를 가져와 쉐이더 생성, 추가적으로 Geometry 쉐이더를 불러올 수 있다)
    //  - ZeroWopShader을 통해 쉽게 쉐이더로 값을 전송할 수 있다.
    //  - ../Resources/ 가 맨 앞에 자동으로 붙어서 경로를 생성한다.
    ZeroShader* LoadShaders  (std::string name, std::string vertex, std::string fragment, std::string geometry = "");
    ZeroShader* GetShader    (std::string name);


    //-=Load Font=-
    // * 폰트 로더
    //  - 폰트 경로를 통해 ttf타입의 폰트를 불러온다.
    //  - ../Resources/ 가 맨 앞에 자동으로 붙어서 경로를 생성한다.
    ZeroFont* LoadFont        (std::string name);


    //-=Load Audio=-
    // * 오디오 로더
    //  - 오디오 경로를 통해 wav타입의 오디오 소스를 불러온다.
    //  - 경로를 입력하면 ALuint(unsigned int)형으로 ID가 반환된다.
    //  - ../Resources/ 가 맨 앞에 자동으로 붙어서 경로를 생성한다.
    //
    //-=UnLoadAudio=-
    // * 오디오 언로더
    // - 사용되지 않을 오디오의 ID값(ALuint형)을 통해 뷸러온 오디오를 지운다
    ALuint LoadAudio(std::string path);
    bool UnLoadAudio(const ALuint& bufferId);


    //-= Set Resources Base Path =-
    // - 리소스 폴더의 위치가 실행파일을 기준으로 어디에 있는지 경로를 설정해줌
    void SetResourcesBasePath(std::string path = "../Resources/") { SAFE_DELETE(basePath); basePath = new std::string(path); }
    std::string SetResourcesBasePath() {return (*basePath);}

    //-= Release =-
    // - 불러온 쉐이더와 텍스쳐, 사운드, 폰드를 지움
    void Release();

private:
    std::string                           *  basePath   = nullptr;
    std::map<std::string, ZeroFont*>      *  fonts      = nullptr;
    std::map<std::string, ZeroTexture*>   *  textures   = nullptr;
    std::map<std::string, ZeroShader*>    *  shaders    = nullptr;
    std::vector<ALuint>                   *  sounds     = nullptr;
    
};


#endif //ZERO_ENGINE_ZERORESOURCESMANAGER_H
