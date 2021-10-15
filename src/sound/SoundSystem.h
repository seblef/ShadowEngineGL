#pragma once

#include "SoundVirtualSource.h"


class SoundSystem : public TSingleton<SoundSystem>
{
	protected:

		typedef vector<ISoundSource*>		SourceVec;
		typedef vector<SoundVirtualSource*> VirtualSourceVec;
		typedef set<SoundVirtualSource*>	VirtualSourceSet;

		ISoundDevice*			_device;

		TDataBase<ISound>		_soundStore;

		float					_totalTime;
		SourceVec				_totalSources;
		SourceVec				_freeSources;
		VirtualSourceSet		_managedSources;
		VirtualSourceVec		_release;

		VirtualSourceVec		_playingSources;
		
		void					updateSources();
		void					updateVirtualSources();
		void					clearStopSounds();
		void					stopPlayingSources();
		void					playUnplayedSources();
		void					clearManagedSources();

	public:

		SoundSystem(ISoundDevice* d);
		~SoundSystem();

        bool					update(float time);
		void					reset();

		ISound*					loadSound(const string& soundFile);
		void					releaseSound(ISound* buffer);

		SoundVirtualSource*		play(
			ISound* buffer,
			int priority,
			float gain,
			const Vector3& position,
			const Vector3& velocity,
			const Vector3& direction,
			float refDist, float maxDist,
			float rollOff,
			float inAngle,
			float outAngle,
			bool managed=false,
			bool loop=false
		);
		void					play(SoundVirtualSource* src, ISound* buffer);
		void					stopSound(SoundVirtualSource *virtualSource);

		int						getAvailableChannels()								{ return _freeSources.size(); }
		int						getPlayingChannels()								{ return _playingSources.size(); }

		ISoundDevice*			getAudio() const									{ return _device;  }
};
