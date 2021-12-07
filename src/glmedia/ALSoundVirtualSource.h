#pragma once

#include "../mediacommon/ISoundVirtualSource.h"
#include "../core/Vector3.h"

using namespace Core;


class ALSoundVirtualSource : public ISoundVirtualSource
{
protected:

    Vector3 _position;
    Vector3 _velocity;
    Vector3 _direction;
    float _gain;
    float _referenceDistance;
    float _maxDistance;
    float _rollOff;
    float _inAngle;
    float _outAngle;
    bool _relative;
    unsigned int _playingSource;

public:

    ALSoundVirtualSource();
    ALSoundVirtualSource(
        const Vector3& pos,
        const Vector3& vel,
        const Vector3& dir,
        float refDist, float maxDist,
        float rollOff,
        float inAngle, float outAngle
    );
    ~ALSoundVirtualSource()             {}

	void		setPosition(const Vector3& pos);
	void		setVelocity(const Vector3& vel);
	void		setDirection(const Vector3& dir);
	void		setGain(float gain);
	void		setReferenceDistance(float refDist);
	void		setRollOff(float rollOff);
	void		setMaxDistance(float maxDist);
	void		setRelative(bool relative);
	void		setConeAngles(float inAngle, float outAngle);
    void        setPlayingSource(unsigned int src);

	const Vector3&		getPosition() const  { return _position; }
	const Vector3&		getVelocity() const { return _velocity; }
	const Vector3&		getDirection() const { return _direction; }
	float				getGain() const { return _gain; }
	float 				getReferenceDistance() const { return _referenceDistance; }
	float				getRollOff() const { return _rollOff; }
	float				getMaxDistance() const { return _maxDistance; }
	float				getConeInnerAngle() const { return _inAngle; }
	float				getConeOuterAngle() const { return _outAngle; }
	bool				isRelative() const { return _relative; }
    unsigned int        getPlayingSource() const { return _playingSource; }
};
