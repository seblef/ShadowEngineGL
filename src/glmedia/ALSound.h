#pragma once

#include "../MediaCommon.h"


class ALSoundInstance
{
protected:

    unsigned int _source;
    unsigned int _buffer;

public:

    ALSoundInstance(unsigned int source, unsigned int buffer) :
        _source(source),
        _buffer(buffer)
    {}
    ~ALSoundInstance() {}

    void play(bool loop);
    void stop();
    void update();
};


class ALSound : public ISound
{
protected:

    int _buffer;

public:

    ALSound(const string& soundName);
    ~ALSound();

    ALSoundInstance* createInstance(unsigned int source);
};
