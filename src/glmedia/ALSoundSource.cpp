#include "ALSoundSource.h"
#include "ALSoundVirtualSource.h"
#include "ALSound.h"
#include "../loguru.hpp"
#include <AL/al.h>


ALSoundSource::ALSoundSource() :
    _source(0),
    _sound(0),
    _virtualSource(0)
{
    alGetError();
    alGenSources(1, (ALuint*)&_source);
    if(alGetError() != AL_NO_ERROR)
        LOG_S(ERROR) << "Failed creating sound source";
}

ALSoundSource::~ALSoundSource()
{
    stop();
    if(_source)
        alDeleteSources(1, (ALuint*)&_source);
}

void ALSoundSource::play(
    ISoundVirtualSource *src,
    ISound* sound,
    bool loop
)
{
    stop();
    _virtualSource = (ALSoundVirtualSource*)src;
    _sound = ((ALSound*)sound)->createInstance(_source);
    _virtualSource->setPlayingSource(_source);
    _sound->play(loop);
}

void ALSoundSource::stop()
{
    if(_virtualSource)
    {
        _virtualSource->setPlayingSource(0);
        _virtualSource = 0;
    }

    if(_sound)
    {
        _sound->stop();
        delete _sound;
        _sound = 0;
    }
}

bool ALSoundSource::isPlaying()
{
    if(!_source)
        return false;
    ALint state = AL_STOPPED;
    alGetSourcei(_source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void ALSoundSource::update()
{
    if(_source && _sound)
        _sound->update();
}