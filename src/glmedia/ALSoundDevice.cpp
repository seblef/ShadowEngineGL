#include "ALSoundDevice.h"
#include "../loguru.hpp"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>


#define SOURCES_COUNT               16



static char* checkString(char* x)
{
    return (char*)(x ? x : "Unknown");
}


ALSoundDevice::ALSoundDevice() :
    _sources(0),
    _sourcesCount(SOURCES_COUNT)
{
    ALCdevice *device = alcOpenDevice(0);
    if(!device)
    {
        LOG_S(ERROR) << "Failed opening OpenAL device";
        return;
    }

    ALCcontext *context = alcCreateContext(device, 0);
    if(!context)
    {
        LOG_S(ERROR) << "Failed creating OpenAL context";
        alcCloseDevice(device);
    }

    alcMakeContextCurrent(context);
    alDistanceModel(AL_INVERSE_DISTANCE);

    LOG_SCOPE_F(INFO, "Sound device infos");
    LOG_S(INFO) << "AL_VENDOR: " << checkString((char*)alGetString(AL_VENDOR));
    LOG_S(INFO) << "AL_VERSION: " << checkString((char*)alGetString(AL_VERSION));
    LOG_S(INFO) << "AL_RENDERER: " << checkString((char*)alGetString(AL_RENDERER));
    LOG_S(INFO) << "AL_EXTENSIONS: " << checkString((char*)alGetString(AL_EXTENSIONS));

    if(alutInitWithoutContext(0, 0) != AL_TRUE)
        LOG_S(ERROR) << "Failed initializing alut";

    _sources = new ALSoundSource[_sourcesCount];
}

ALSoundDevice::~ALSoundDevice()
{
    if(_sources)
    {
        for(int i=0; i<_sourcesCount; ++i)
            _sources[i].stop();
        delete[] _sources;
    }

    ALCcontext *context = alcGetCurrentContext();
    ALCdevice *device = alcGetContextsDevice(context);

    alcDestroyContext(context);
    alcCloseDevice(device);
}

ISound* ALSoundDevice::loadSound(const string& soundFile)
{
    return new ALSound(soundFile);
}

void ALSoundDevice::update()
{
    for(int i=0; i<_sourcesCount; ++i)
        _sources[i].update();
}

ISoundVirtualSource* ALSoundDevice::createVirtualSource(
    const Vector3& pos,
    const Vector3& vel,
    const Vector3& dir,
    float refDist, float maxDist,
    float rollOff,
    float inAngle, float outAngle
) const
{
    return new ALSoundVirtualSource(
        pos, vel, dir,
        refDist, maxDist,
        rollOff,
        inAngle, outAngle
    );
}