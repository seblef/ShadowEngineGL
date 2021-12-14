#pragma once

#include "TextureSet.h"
#include "../mediacommon/IVideoDevice.h"
#include <memory>


class ParticleMaterial
{
protected:

	IVideoDevice* _device;

	ITexture* _texture;
	Particles::TextureSet* _textureSet;

	BlendState _blendState;
	BlendMode getBlendMode(const string& bm) const;

public:

	ParticleMaterial(const ParticleMaterial& m);
	ParticleMaterial(const YAML::Node& node);
	ParticleMaterial(const string& texFile, bool fileIsDir,
		BlendMode srcBlend=BLEND_SRCALPHA,
		BlendMode destBlend=BLEND_INVSRCALPHA);
	ParticleMaterial(const vector<string>& texSet,
		BlendMode srcBlend=BLEND_SRCALPHA,
		BlendMode destBlend=BLEND_INVSRCALPHA);
	~ParticleMaterial();

	ITexture*				getTexture() const					{ return _texture; }
	Particles::TextureSet*		getTextureSet() const				{ return _textureSet; }

	ITexture*				getTexture(float percent) const;
	BlendState				getBlendState() const				{ return _blendState; }
};

