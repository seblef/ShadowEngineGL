
#include "GameSound.h"


GameSound::GameSound(
	TemplateSound* snd,
	const Matrix4& world
) :
	GameObject(SOUND,world, BBox3::NullBox, 0),
	_playing(false),
	_template(snd)
{
	_source = new SoundVirtualSource(
		SoundSystem::getSingletonRef().getAudio(),
		snd->getPriority(),
		snd->getGain(),
		true,
		Vector3::NullVector,
		Vector3::NullVector,
		Vector3::ZAxisVector,
		snd->getReferenceDistance(),
		snd->getMaxDistance(),
		1.f,
		snd->getInnerAngle(), snd->getOuterAngle()
	);
	_source->setSound(snd->getSound());

	onUpdateMatrix(world);
}

GameSound::~GameSound()
{
	if (_playing)			stop();
	delete _source;
}

void GameSound::play()
{
	if (!_playing)
	{
		_playing = true;
		SoundSystem::getSingletonRef().play(_source, _template->getSound());
	}
}

void GameSound::stop()
{
	if (_playing)
	{
		_playing = false;
		SoundSystem::getSingletonRef().stopSound(_source);
	}
}

void GameSound::onUpdateMatrix(const Matrix4& world)
{
	ISoundVirtualSource* src = _source->getSource();
    src->setPosition((const Vector3&)world);
	src->setVelocity(Vector3::NullVector);

	Vector3 dir(Vector3::ZAxisVector);
	dir.transformNoTranslation(world);

	src->setDirection(dir);
}
