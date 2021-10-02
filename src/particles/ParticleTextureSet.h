#pragma once

#include "../MediaCommon.h"


class ParticleTextureSet
{
	protected:

		typedef vector<ITexture*>		TexVector;
		TexVector						_textures;
		IVideoDevice*					_device;

	public:

		ParticleTextureSet(IVideoDevice* d) : _device(d)			{}
		ParticleTextureSet(const ParticleTextureSet& s);
		ParticleTextureSet(ScriptFile& sf, IVideoDevice* d);
		ParticleTextureSet(const string& dir, IVideoDevice* d);
		~ParticleTextureSet();

		void							addTexture(const string& texFile);
		int								getTextureCount() const					{ return _textures.size(); }
		ITexture*						getTexture(const int n) const			{ return _textures[n]; }
		ITexture*						operator[](size_t n) const				{ return getTexture((int)n); }
};
