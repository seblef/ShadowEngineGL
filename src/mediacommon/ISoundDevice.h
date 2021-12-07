#pragma once

class ISoundSource;
class ISoundVirtualSource;
class ISoundListener;
class ISound;

namespace Core
{
    class Vector3;
}

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
		const Core::Vector3& pos,
		const Core::Vector3& vel,
		const Core::Vector3& dir,
		float refDist, float maxDist,
		float rollOff,
		float inAngle, float outAngle
	) const = 0;
};
