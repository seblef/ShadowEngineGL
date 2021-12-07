#pragma once

#include "../mediacommon/ISoundSource.h"

class ALSoundInstance;
class ALSoundVirtualSource;

class ALSoundSource : public ISoundSource
{
protected:

    unsigned int _source;
    ALSoundInstance* _sound;
    ALSoundVirtualSource* _virtualSource;

public:

    ALSoundSource();
    ~ALSoundSource();

    void			play(
        ISoundVirtualSource* src,
        ISound* sound,
        bool loop = false
    );
    void			stop();
    bool			isPlaying();
    void            update();
};
