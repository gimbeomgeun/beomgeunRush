//
// Created by EunwooSong on 2021-01-23.
//

#include "stdafx.h"
#include "ZeroSoundDevice.h"
#include "ZeroAudioLogger.h"
#include "ZeroLogger.h"
#include "ZeroMath.h"
#include <al.h>

ZeroSoundDevice::ZeroSoundDevice() {
    p_alcDevice = alcOpenDevice(nullptr);
    if (!p_alcDevice) {
        ZERO_LOGGER::Error("failed to get sound device");
        throw("failed to get sound device");
    }


    p_alcContext = alcCreateContext(p_alcDevice, nullptr);
    if(!p_alcContext) {
        ZERO_LOGGER::Error("failed to get sound device");
        throw("failed to get sound device");
    }


    if(!alcMakeContextCurrent(p_alcContext)) {
        ZERO_LOGGER::Error("failed to make context current");
        throw("failed to make context current");
    }


    const ALCchar* name = nullptr;
    if(alcIsExtensionPresent(p_alcDevice, "ALC_ENUMERATE_ALL_EXT"))
        name = alcGetString(p_alcDevice, ALC_ALL_DEVICES_SPECIFIER);
    if(!name || alcGetError(p_alcDevice) != AL_NO_ERROR)
        name  = alcGetString(p_alcDevice, ALC_DEVICE_SPECIFIER);

    ZERO_LOGGER::Info("Opened Sound Device : %s", name);

    if (alGetError() != AL_NO_ERROR)
        ZERO_LOGGER::Error("Something is wrong . . . %d", alGetError());
}

ZeroSoundDevice::~ZeroSoundDevice() {
    ReleaseDevice();
}

//void ZeroSoundDevice::Initialize() {
//
//}

void ZeroSoundDevice::GetLocation(float &x, float &y, float &z) {
    alGetListener3f(AL_POSITION, &x, &y, &z);
    AL_CheckAndThrow();
}

void ZeroSoundDevice::GetOrientation(float &ori) {
    alGetListenerfv(AL_ORIENTATION, &ori);
}

float ZeroSoundDevice::GetGain() {
    float currentGain;
    alGetListenerf(AL_GAIN, &currentGain);
    AL_CheckAndThrow();
    return currentGain;
}

void ZeroSoundDevice::SetAttunation(int key) {
    if(key < 0xD001 || key > 0xD006)
        throw("Bad attnation key");

    alDistanceModel(key);
    AL_CheckAndThrow();
}

void ZeroSoundDevice::SetLocation(const float x, const float y) {
    alListener3f(AL_POSITION, x, y, 0);
    AL_CheckAndThrow();
}

void ZeroSoundDevice::SetLocation(const float x, const float y, const float z) {
    alListener3f(AL_POSITION, x, y, z);
    AL_CheckAndThrow();
}

void ZeroSoundDevice::SetOrientation(const float &atx, const float &aty, const float &atz, const float &upx,
                                     const float &upy, const float &upz) {
    std::vector<float> ori;

    ori.push_back(atx);
    ori.push_back(aty);
    ori.push_back(atz);
    ori.push_back(upx);
    ori.push_back(upy);
    ori.push_back(upz);

    alListenerfv(AL_ORIENTATION, ori.data());
    AL_CheckAndThrow();
}

void ZeroSoundDevice::SetGain(const float val) {
// clamp between 0 and 5
    float newVol = ZeroMath::Clamp(val, 0.0f, 5.0f);

    alListenerf(AL_GAIN, newVol);
    AL_CheckAndThrow();
}

void ZeroSoundDevice::ReleaseDevice() {
    if(!isSoundRelease) {
        p_alcContext = alcGetCurrentContext();
        p_alcDevice = alcGetContextsDevice(p_alcContext);

        if (!alcMakeContextCurrent(NULL))
            ZERO_LOGGER::Error("Failed to set context to nullptr");

        alcDestroyContext(p_alcContext);
        if (!alcCloseDevice(p_alcDevice))
            ZERO_LOGGER::Error("Failed to close sound device");
    }

    

    isSoundRelease = true;
}
