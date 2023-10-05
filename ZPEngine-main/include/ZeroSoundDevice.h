//
// Created by EunwooSong on 2021-01-23.
//

#ifndef ZERO_ENGINE_ZEROSOUNDDEVICE_H
#define ZERO_ENGINE_ZEROSOUNDDEVICE_H

#include <AL/alc.h>

class ZeroSoundDevice {
public:
    ZeroSoundDevice();
    ~ZeroSoundDevice();
    //void Initialize();

    void GetLocation(float &x, float &y, float &z);
    void GetOrientation(float &ori);
    float GetGain();

    void SetAttunation(int key);
    void SetLocation(const float x, const float y);
    void SetLocation(const float x, const float y, const float z);
    void SetOrientation(const float& atx, const float& aty, const float& atz,
                        const float& upx, const float& upy, const float& upz);
    void SetGain(const float val);

    void ReleaseDevice();

private:
    ALCdevice* p_alcDevice;
    ALCcontext* p_alcContext;

    bool isSoundRelease = false;
};


#endif //ZERO_ENGINE_ZEROSOUNDDEVICE_H
