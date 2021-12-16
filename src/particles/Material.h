#pragma once

#include "../mediacommon/IVideoDevice.h"
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
	TextureSet* _textureSet;

	BlendState _blendState;
	BlendMode getBlendMode(const string& bm) const;

    void cleanUp();

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
    void setTexture(const std::string& texture);

	TextureSet* getTextureSet() const { return _textureSet; }
    void setTextureSet(const std::string& folder);

	ITexture* getTexture(float percent) const;

	BlendState getBlendState() const { return _blendState; }
    void setBlendState(BlendState blendState) { _blendState = blendState; }
};

}