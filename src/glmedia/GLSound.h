#ifndef GLSOUND_H
#define GLSOUND_H

#include "../MediaCommon.h"

class GLSound : public ISound
{
public:

    GLSound(const string& soundName) : ISound(soundName)        {}
    ~GLSound()                                                  {}
};

#endif // GLSOUND_H
