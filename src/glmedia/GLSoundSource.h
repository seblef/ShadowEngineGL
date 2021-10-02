#pragma once

#include "../MediaCommon.h"

class GLSoundSource : public ISoundSource
{
public:

    GLSoundSource()         {}
    ~GLSoundSource()        {}

    void			play(ISoundVirtualSource* virtSrc, ISound* buffer, bool loop = false)       {}
    void			stop()                                                                      {}

    bool			isPlaying()                                                                 { return false; }
};
