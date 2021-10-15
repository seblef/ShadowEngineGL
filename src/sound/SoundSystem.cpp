
#include "SoundSystem.h"
#include "SoundPlayingSource.h"
#include "SoundVirtualSource.h"
#include "../loguru.hpp"
#include <algorithm>


// #define _DEBUG_SOUND


SoundSystem::SoundSystem(ISoundDevice* d) : _totalTime(0.0f), _device(d)
{
	for (int i = 1; i<d->getSourceCount(); ++i)
	{
		_totalSources.push_back(d->getSource(i));
		_freeSources.push_back(d->getSource(i));
	}
}

SoundSystem::~SoundSystem()
{
}

bool SoundSystem::update(float time)
{
	_totalTime += time;
	if(_totalTime >= 0.2f)
	{
		_totalTime = 0.0f;
		updateSources();
	}

	return true;
}

static inline bool lt_virt(SoundVirtualSource *v2, SoundVirtualSource *v1)
{ 
	float dist1 = 0.0f;
	float dist2 = 0.0f;
	unsigned int priority1 = 0;
	unsigned int priority2 = 0;

	if (v1 && !v1->isStopped()) priority1 = v1->getPriority();
	if (v2 && !v2->isStopped()) priority2 = v2->getPriority();
	if (v1) dist1 = v1->getDistance();
	if (v2) dist2 = v2->getDistance();

	return (priority1 < priority2 ||
		(priority1 == priority2 && dist1 > dist2));
}

void SoundSystem::updateSources()
{
	updateVirtualSources();

	sort(_playingSources.begin(), _playingSources.end(), lt_virt);

	clearStopSounds();
	clearManagedSources();
	stopPlayingSources();
	playUnplayedSources();
}

void SoundSystem::clearStopSounds()
{
	if (_playingSources.empty())
		return;

	SoundVirtualSource *src;

	while (!_playingSources.empty() && _playingSources.back()->isStopped())
	{
		src = _playingSources.back();
		if (src->getPlayingSource())
		{
			src->getPlayingSource()->stop();
#ifdef _DEBUG_SOUND
			LOG_S(2) << "clearStopSounds: " << src->getPlayingSource();
#endif
			_freeSources.push_back(src->getPlayingSource());
			src->setPlayingSource(0);
		}

		_playingSources.pop_back();
	}
}

void SoundSystem::stopPlayingSources()
{
	int totalSources = _totalSources.size();
	int totalPlaying = _playingSources.size();

	for (int i = totalSources; i < totalPlaying; ++i)
	{
		SoundVirtualSource* src = _playingSources[i];
		if (src->isManaged())
			src->setStopped();
		else if (src->getPlayingSource())
		{
			src->getPlayingSource()->stop();
#ifdef _DEBUG_SOUND
			LOG_S(2) << "stopPlayingSources: " << src->getPlayingSource();
#endif
			_freeSources.push_back(src->getPlayingSource());
			src->setPlayingSource(0);
		}
	}
}

void SoundSystem::playUnplayedSources()
{
	VirtualSourceVec::iterator vs(_playingSources.begin());
	int count = smin((int)_totalSources.size(), (int)_playingSources.size());
	for (int i = 0; i < count; ++i,++vs)
	{
		SoundVirtualSource* src = *vs;
		if (!src->getPlayingSource() && !_freeSources.empty())
		{
			ISoundSource *s = _freeSources.back();
			s->play(src->getSource(), src->getSound(), src->isLooping());
			src->setPlayingSource(s);

			_freeSources.pop_back();
		}
	}
}

void SoundSystem::updateVirtualSources()
{
	Vector3 pos(_device->getListener()->getPosition());
	int totalSources = _totalSources.size();

	int j=0;
	for(auto & source : _playingSources)
	{
		if (j < totalSources && source->getPlayingSource() && !source->getPlayingSource()->isPlaying())
			source->setStopped();

		source->updateDistance(pos);
		++j;
	}
}

void SoundSystem::clearManagedSources()
{
	for(auto & source : _managedSources)
	{
		if (source->isStopped())
			_release.push_back(source);
	}

	for(auto & source : _release)
	{
		_managedSources.erase(source);
		delete source;
	}

	_release.clear();
}

ISound* SoundSystem::loadSound(const string& soundFile)
{
	ISound *snd = _soundStore.getData(soundFile);
	if (snd)
		snd->addRef();
	else
	{
		snd = _device->loadSound(soundFile);
		if (snd)
			_soundStore.registerData(soundFile, snd);
	}

	return snd;
}

void SoundSystem::releaseSound(ISound* buffer)
{
	if (buffer->remRef() < 1)
	{
		_soundStore.unregisterData(buffer->getSoundName());
		delete buffer;
	}
}

SoundVirtualSource *SoundSystem::play(
	ISound* buffer,
	int priority,
	float gain,
	const Vector3& position,
	const Vector3& velocity,
	const Vector3& direction,
	float refDist, float maxDist,
	float rollOff,
	float inAngle, float outAngle,
	bool managed,
	bool loop
)
{
	SoundVirtualSource *src = new SoundVirtualSource(
		_device,
		priority,
		gain,
		loop,
		position,
		velocity,
		direction,
		refDist, maxDist,
		rollOff,
		inAngle, outAngle,
		managed
	);
	play(src, buffer);
	return src;
}

void SoundSystem::play(SoundVirtualSource* src, ISound* buffer)
{
	src->setSound(buffer);
	_playingSources.push_back(src);

	updateSources();

	if (src->isManaged())
		_managedSources.insert(src);
}

void SoundSystem::stopSound(SoundVirtualSource *virtualSource)
{
	virtualSource->setStopped();
	updateSources();
}

void SoundSystem::reset()
{
	for(auto & source : _playingSources)
		source->setStopped();
	updateSources();
}
