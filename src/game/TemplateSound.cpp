
#include "TemplateSound.h"
#include "GameSystem.h"

TemplateSound::TemplateSound(
	ISound *snd,
	float maxDist, float refDist,
	float gain,
	float inAngle, float outAngle,
	int priority,
	bool autoPlay
) :
	_sound(snd),
	_maxDistance(maxDist),
	_referenceDistance(refDist),
	_gain(gain),
	_inAngle(inAngle),
	_outAngle(outAngle),
	_priority(priority),
	_autoPlay(autoPlay)
{
}

TemplateSound::~TemplateSound()
{
	if (_sound)
		SoundSystem::getSingletonRef().releaseSound(_sound);
}
