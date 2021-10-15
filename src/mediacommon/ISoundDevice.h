#pragma once

#include "ISoundSource.h"
#include "ISoundVirtualSource.h"
#include "ISoundListener.h"

class ISoundDevice
{
public:

	ISoundDevice()				{}
	virtual ~ISoundDevice()		{}

	virtual ISound*						loadSound(const string& soundFile)=0;

	virtual void						update()=0;

	virtual ISoundListener*				getListener() = 0;

	virtual int							getSourceCount() const = 0;
    virtual ISoundSource*				getSource(int n) = 0;

	virtual ISoundVirtualSource*		createVirtualSource() const = 0;
	virtual ISoundVirtualSource*		createVirtualSource(
		const Vector3& pos,
		const Vector3& vel,
		const Vector3& dir,
		float refDist, float maxDist,
		float rollOff,
		float inAngle, float outAngle
	) const = 0;
};
