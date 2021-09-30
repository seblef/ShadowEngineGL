#ifndef GLSOUNDCONE_H
#define GLSOUNDCONE_H

#include "../MediaCommon.h"

class GLSoundCone : public ISoundCone
{
public:

    GLSoundCone()           {}
    ~GLSoundCone()          {}

    void			setInnerAngle(float a)              {}
    void			setOuterAngle(float a)              {}
    void			setInnerVolume(float v)             {}
    void			setOuterVolume(float v)             {}
    void			setInnerLPF(float lpf)              {}
    void			setOuterLPF(float lpf)              {}
    void			setInnerReverb(float r)             {}
    void			setOuterReverb(float r)             {}

    float			getInnerAngle()                     { return 0.0f; }
    float			getOuterAngle()                     { return 1.0f; }
    float			getInnerVolume()                    { return 0.0f; }
    float			getOuterVolume()                    { return 1.0f; }
    float			getInnerReverb()                    { return 0.0f; }
    float			getOuterReverb()                    { return 1.0f; }
    float			getInnerLPF()                       { return 0.0f; }
    float			getOuterLPF()                       { return 1.0f; }

};

#endif // GLSOUNDCONE_H
