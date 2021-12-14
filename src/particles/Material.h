#pragma once

#include "../mediacommon/IVideoDevice.h"
#include <memory>
#include <vector>

namespace YAML
{
class Node;
}

namespace Particles
{

class TextureSet;

class Material
{
protected:

	IVideoDevice* _device;

	ITexture* _texture;
	std::unique_ptr<TextureSet> _textureSet;

	BlendState _blendState;
	BlendMode getBlendMode(const string& bm) const;

public:

	Material(const Material& m);
	Material(const YAML::Node& node);
	Material(const std::string& texFile, bool fileIsDir,
		BlendMode srcBlend=BLEND_SRCALPHA,
		BlendMode destBlend=BLEND_INVSRCALPHA);
	Material(const std::vector<std::string>& texSet,
		BlendMode srcBlend=BLEND_SRCALPHA,
		BlendMode destBlend=BLEND_INVSRCALPHA);
	~Material();

	ITexture* getTexture() const { return _texture; }
	TextureSet* getTextureSet() const { return _textureSet.get(); }

	ITexture* getTexture(float percent) const;
	BlendState getBlendState() const { return _blendState; }
};

}