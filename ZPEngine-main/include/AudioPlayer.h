//
// Created by EunwooSong on 2021-01-24.
//

#ifndef ZERO_ENGINE_AUDIOPLAYER_H
#define ZERO_ENGINE_AUDIOPLAYER_H

#include "ZeroComponent.h"
#include <AL\al.h>

//-= AudioPlayer =-
// - 사운드를 출력하기위한 컴포넌트
class AudioPlayer : public Component {
public:
    AudioPlayer();
    ~AudioPlayer();

    void Update() override {};
    void Start() override {};

    //사운드 버퍼를 출력을 시작하는 함수
    void Play();

    //사운드 버퍼를 멈추는 함수
    void Stop();

    //사운드 버퍼를 일시정지하는 함수
    void Pause();

    //사운드 버퍼 출력을 계속 시작하는 함수
    void Resume();

    //사운드 버퍼를 설정하는 함수
    void SetPlayBuffer(const ALuint& id);

    //사운드 버퍼의 반복여부를 설정하는 함수
    void SetLooping(const bool& loop);
    
    //사운드 버퍼의 위치를 설정하는 함수
    void SetPosition(const float& x, const float& y, const float& z);

    //현재 사운드 버퍼가 실행중인지 확인하는 함수
    bool isPlaying();

private:
    ALuint source;
    ALuint buffer;

    bool autoPlay;
};


#endif //ZERO_ENGINE_AUDIOPLAYER_H
