//
// Created by EunwooSong on 2021-01-24.
//

#include "stdafx.h"
#include "AudioPlayer.h"
#include "ZeroAudioLogger.h"
#include "ZeroSystem.h"

AudioPlayer::AudioPlayer() : source(0), buffer(0) {
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    AL_CheckAndThrow();
}

AudioPlayer::~AudioPlayer() {
    Stop();

    alSourcei(source, AL_BUFFER, 0);
    alDeleteSources(1, &source);
    ZERO_RESOURCES_MGR->UnLoadAudio(source); //UnLoad Sound Buffer...
}

void AudioPlayer::Play() {
    alSourcePlay(source);
    AL_CheckAndThrow();
}

void AudioPlayer::Stop() {
    alSourceStop(source);
    AL_CheckAndThrow();
}

void AudioPlayer::Pause() {
    alSourcePause(source);
    AL_CheckAndThrow();
}

void AudioPlayer::Resume() {
    alSourcePlay(source);
    AL_CheckAndThrow();
}

void AudioPlayer::SetPlayBuffer(const ALuint &id) {
    if (id != buffer)
    {
        buffer = id;
        alSourcei(source, AL_BUFFER, (ALint)id);
        AL_CheckAndThrow();
    }
}

void AudioPlayer::SetLooping(const bool &loop) {
    alSourcei(source, AL_LOOPING, (ALint)loop);
    AL_CheckAndThrow();
}

void AudioPlayer::SetPosition(const float &x, const float &y, const float &z) {
    alSource3f(source, AL_POSITION, x, y, z);
    AL_CheckAndThrow();
}

bool AudioPlayer::isPlaying() {
    ALint playState;
    alGetSourcei(source, AL_SOURCE_STATE, &playState);
    return (playState == AL_PLAYING);
}
