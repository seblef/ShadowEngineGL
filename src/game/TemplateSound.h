
#pragma once


class ISound;

class TemplateSound
{
protected:

	ISound*				_sound;
	float				_maxDistance;
	float				_referenceDistance;
	float				_gain;
	float 				_inAngle;
	float				_outAngle;
	int					_priority;
	bool				_autoPlay;

public:

	TemplateSound(
		ISound *snd,
		float maxDist, float refDist,
		float gain,
		float inAngle, float outAngle,
		int priority,
		bool autoPlay
	);
	~TemplateSound();

	ISound*				getSound() const			{ return _sound; }
	float				getMaxDistance() const		{ return _maxDistance; }
	float				getReferenceDistance() const { return _referenceDistance; }
	float				getGain() const				{ return _gain; }
	float				getInnerAngle() const		{ return _inAngle; }
	float				getOuterAngle() const		{ return _outAngle; }
	int					getPriority() const			{ return _priority; }
	bool				getAutoPlay() const			{ return _autoPlay; }
};
