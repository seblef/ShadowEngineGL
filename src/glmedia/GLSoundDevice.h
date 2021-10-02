#pragma once

#include "GLSoundListener.h"
#include "GLSoundSource.h"
#include "GLSoundVirtualSource.h"
#include "GLSoundCone.h"
#include "GLSound.h"

class GLSoundDevice : public ISoundDevice
{
protected:

    GLSoundListener         _listener;
    GLSoundSource           _sources[16];

public:

    GLSoundDevice()             {}
    ~GLSoundDevice()            {}

    ISound*						loadSound(const string& soundFile)  { return new GLSound(soundFile); }

    void						update()                            {}

    ISoundListener*				getListener()                       { return &_listener; }

    int							getSourceCount() const              { return 16; }
    ISoundSource*				getSource(int n)                    { return &_sources[n]; }

    ISoundCone*					createCone() const                  { return new GLSoundCone; }
    ISoundCone*					createCone(float iAngle, float oAngle, float iVolume, float oVolume,
        float iLPF, float oLPF, float iReverb, float oReverb) const { return new GLSoundCone; }

    ISoundVirtualSource*		createVirtualSource() const         { return new GLSoundVirtualSource; }
    ISoundVirtualSource*		createVirtualSource(const Vector3& pos, const Vector3& vel,
        float inRadius, float inRadiusAngle, float volume=1.0f,
        ISoundCone *cone=0, const Vector3& frontAxis = Vector3::ZAxisVector,
        const Vector3& topAxis = Vector3::YAxisVector,
        float dopplerScale = 1.0f) const                            { return new GLSoundVirtualSource; }
};
