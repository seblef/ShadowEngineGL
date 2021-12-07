#pragma once

namespace Core
{
    class Vector3;
}

using namespace Core;



class ISoundVirtualSource
{
public:

	ISoundVirtualSource() {}
	virtual ~ISoundVirtualSource()	{}

	virtual void setPosition(const Vector3& pos) = 0;
	virtual void setVelocity(const Vector3& vel) = 0;
	virtual void setDirection(const Vector3& dir) = 0;
	virtual void setGain(float gain) = 0;
	virtual void setReferenceDistance(float refDist)= 0 ;
	virtual void setRollOff(float rollOff) = 0;
	virtual void setMaxDistance(float maxDist) = 0;
	virtual void setRelative(bool relative) = 0;
	virtual void setConeAngles(float inAngle, float outAngle) = 0;

	virtual const Vector3& getPosition() const = 0;
	virtual const Vector3& getVelocity() const = 0;
	virtual const Vector3& getDirection() const = 0;
	virtual float getGain() const = 0;
	virtual float getReferenceDistance() const = 0;
	virtual float getRollOff() const = 0;
	virtual float getMaxDistance() const = 0;
	virtual float getConeInnerAngle() const = 0;
	virtual float getConeOuterAngle() const = 0;
	virtual bool isRelative() const = 0;
};
