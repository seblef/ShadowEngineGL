
#include "Material.h"
#include "ParticleEngine.h"
#include "TextureSet.h"
#include "../core/CoreCommon.h"
#include "../mediacommon/ITexture.h"
#include <yaml-cpp/yaml.h>

const char * const g_ParticleBlendNames[]=
{
    "zero",
    "one",
    "src_color",
    "inv_src_color",
    "src_alpha",
    "inv_src_alpha",
    "dest_alpha",
    "inv_dest_alpha",
    "dest_color",
    "inv_dest_color",
    "src_alpha_sat",
    "both_src_alpha",
    "both_inv_src_alpha",
    "blend_factor",
    "inv_blend_factor",
    "src_color2",
    "inv_src_color2"
};

namespace Particles
{

Material::Material(const Material& m) :
    _device(m._device),
    _texture(0)
{
	if (m._textureSet)
		_textureSet = std::unique_ptr<TextureSet>(new TextureSet(*m._textureSet));
	else if (m._texture)
	{
		_texture = m._texture;
		_texture->addRef();
	}

	BlendMode src, dest;
	bool blend;

	ParticleEngine::getSingletonRef().getVideoDevice()->getBlendStateDesc(m._blendState, blend, src, dest);
	_blendState = ParticleEngine::getSingletonRef().getVideoDevice()->createBlendState(blend, src, dest);
}

Material::Material(const YAML::Node& node) :
	_texture(0),
	_blendState(0)
{
	_device = ParticleEngine::getSingletonRef().getVideoDevice();

	if(node["texture"])
		_texture = _device->createTexture(node["texture"].as<string>());
	else if(node["texture_set"])
    {
        YAML::Node tex_set = node["texture_set"];
		_textureSet = std::unique_ptr<TextureSet>(new TextureSet(tex_set, _device));
    }
	else if(node["texture_dir"])
		_textureSet = std::unique_ptr<TextureSet>(new TextureSet(node["texture_dir"].as<string>(), _device));

    YAML::Node blend = node["blend"];
	if(blend)
	{
		BlendMode src, dest;
		src = getBlendMode(blend["src"].as<string>());
		dest = getBlendMode(blend["dest"].as<string>());
		_blendState = _device->createBlendState(true, src, dest);
	}
	else
		_blendState=_device->createBlendState(true,BLEND_SRCALPHA,BLEND_INVSRCALPHA);
}

Material::Material(
    const std::string& texFile,
    bool fileIsDir,
    BlendMode srcBlend,
    BlendMode destBlend
)
{
	_device = ParticleEngine::getSingletonRef().getVideoDevice();

	if (texFile.length() > 0)
	{
		if (fileIsDir)		//  Texture set
			_textureSet = std::unique_ptr<TextureSet>(new TextureSet(texFile, _device));
		else
			_texture = _device->createTexture(texFile);
	}
	else
		_texture=0;

	_blendState = _device->createBlendState(true, srcBlend, destBlend);
}

Material::Material(
    const std::vector<string>& texSet,
    BlendMode srcBlend,
    BlendMode destBlend
) : _texture(0)
{
	_device = ParticleEngine::getSingletonRef().getVideoDevice();

	_textureSet = std::unique_ptr<TextureSet>(new TextureSet(_device));
    for(auto const& t : texSet)
		_textureSet->addTexture(t);

	_blendState = _device->createBlendState(true, srcBlend, destBlend);
}

Material::~Material()
{
	_device->destroyBlendState(_blendState);
	if(_texture)
        _texture->remRef();
}

BlendMode Material::getBlendMode(const string& bm) const
{
	BlendMode b=BLEND_ONE;
	for(int i=0;i<BLEND_COUNT;++i)
	{
		if(bm==g_ParticleBlendNames[i])
			b=(BlendMode)i;
	}

	return b;
}

ITexture* Material::getTexture(float percent) const
{
	if(_textureSet)
	{
		int tex=int((_textureSet->getTextureCount()-1) * percent);
		tex=smin(smax(tex,0),_textureSet->getTextureCount()-1);
		return _textureSet->getTexture(tex);
	}
	else
		return _texture;
}

}
