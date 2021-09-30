#ifndef GLSOUNDSOURCE_H
#define GLSOUNDSOURCE_H

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

#endif // GLSOUNDSOURCE_H
