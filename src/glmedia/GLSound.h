#pragma once

#include "../MediaCommon.h"

class GLSound : public ISound
{
public:

    GLSound(const string& soundName) : ISound(soundName)        {}
    ~GLSound()                                                  {}
};
