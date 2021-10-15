#include "ALSound.h"
#include <AL/al.h>
#include <AL/alut.h>
#include "../loguru.hpp"


void ALSoundInstance::play(bool loop)
{
    if(!_buffer)
        return;

	alSourcei(_source,AL_BUFFER, 0);
	alSourcei(_source,AL_BUFFER, _buffer);
	alSourcei(_source,AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	alSourcePlay(_source);
}

void ALSoundInstance::stop()
{
    if(!_buffer)
        return;
    alSourceStop(_source);
}

void ALSoundInstance::update()
{
}

ALSound::ALSound(const string& name) :
    ISound(name),
    _buffer(0)
{
    _buffer = alutCreateBufferFromFile(name.c_str());
    if(_buffer == 0)
    {
        ALenum error = alutGetError();
        LOG_S(ERROR) << "Failed loading sound file " << name << " (" << alutGetErrorString(error) << ")";
    }
}

ALSound::~ALSound()
{
    if(_buffer)
        alDeleteBuffers(1, (ALuint*)&_buffer);
}

ALSoundInstance* ALSound::createInstance(unsigned int source)
{
    return new ALSoundInstance(source, _buffer);
}
