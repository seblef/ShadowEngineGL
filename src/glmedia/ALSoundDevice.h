#pragma once

#include "ALSoundListener.h"
#include "ALSoundSource.h"
#include "ALSoundVirtualSource.h"
#include "ALSound.h"



class ALSoundDevice : public ISoundDevice
{
protected:

    int                     _sourcesCount;
    ALSoundListener         _listener;
    ALSoundSource           *_sources;

public:

    ALSoundDevice();
    ~ALSoundDevice();

    ISound*						loadSound(const string& soundFile);

    void						update();

    ISoundListener*				getListener()                       { return &_listener; }

    int							getSourceCount() const              { return _sourcesCount; }
    ISoundSource*				getSource(int n)                    { return &_sources[n]; }

    ISoundVirtualSource*		createVirtualSource() const         { return new ALSoundVirtualSource; }
    ISoundVirtualSource*		createVirtualSource(
		const Vector3& pos,
		const Vector3& vel,
		const Vector3& dir,
		float refDist, float maxDist,
		float rollOff,
		float inAngle, float outAngle
    ) const;
};
