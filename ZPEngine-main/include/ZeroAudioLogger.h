//
// Created by EunwooSong on 2021-01-24.
//

#ifndef ZERO_ENGINE_ZEROAUDIOLOGGER_H
#define ZERO_ENGINE_ZEROAUDIOLOGGER_H

#include <AL/alc.h>
#include <AL/al.h>

static void ALC_CheckAndThrow(ALCdevice* device)
{
    if (alcGetError(device) != ALC_NO_ERROR)
    {
        throw("error with alcDevice");
    }
}


static void AL_CheckAndThrow()
{
    if (alGetError() != AL_NO_ERROR)
    {
        throw("error with al");
    }
}

#endif //ZERO_ENGINE_ZEROAUDIOLOGGER_H
