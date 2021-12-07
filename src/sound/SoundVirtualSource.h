#pragma once

#include "../core/Vector3.h"

class ISound;
class ISoundSource;
class ISoundVirtualSource;
class ISoundDevice;
class SoundPlayingSource;

using namespace Core;

class SoundVirtualSource
{
	protected:

		int						_priority;
		ISoundSource*			_playingSource;
		ISoundVirtualSource*	_source;
		ISound*					_sound;

		float					_distance;
		float					_gain;
		bool					_loop;
		bool					_stopped;
		bool					_managed;

	public:

		SoundVirtualSource(
			ISoundDevice *device,
			int priority,
			float gain,
			bool loop,
			const Vector3& position,
			const Vector3& velocity,
			const Vector3& direction,
			float refDist, float maxDist,
			float rollOff,
			float inAngle, float outAngle,
			bool managed=false
		);
		~SoundVirtualSource();

		bool					isPlaying() const				{ return _source != 0; }
		bool					isManaged() const				{ return _managed; }
		bool					isLooping() const				{ return _loop; }
		bool					isStopped() const				{ return _stopped; }
		int						getPriority() const				{ return _priority; }
		ISoundSource*			getPlayingSource()				{ return _playingSource; }
		float					getDistance() const				{ return _distance; }
		ISound*					getSound() const				{ return _sound; }
		ISoundVirtualSource*	getSource() const				{ return _source; }

		void					setStopped()									{ _stopped = true; }
		void					setPlayingSource(ISoundSource *source)			{ _playingSource=source; }
		void					setSound(ISound* snd)							{ _sound = snd;  }
		void					updateDistance(const Vector3& listener);
};
