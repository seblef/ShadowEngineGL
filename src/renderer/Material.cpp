
#include "Material.h"
#include "MaterialAnimationFactory.h"
#include "MaterialSystem.h"
#include "MaterialTextureAnimation.h"
#include "MaterialFadeAnimation.h"
#include "MaterialUVAnimation.h"
#include "../mediacommon/ITexture.h"
#include "../mediacommon/IConstantBuffer.h"



Material::Material(IVideoDevice *d) : _device(d), _flags(0)
{
	for(int i=0;i<TS_COUNT;++i)
	{
		_textures[i]=MaterialSystem::getSingletonRef().getDefaultTexture((TextureSlot)i);
		_defaultTex[i]=true;
	}

	_renderState=d->createRenderState(CULL_BACK);
	_blendState=d->createBlendState(false,BLEND_ONE,BLEND_ZERO);
}

Material::Material(const MaterialCreate& mc, IVideoDevice* device) : _device(device), _buffer(mc), _renderState(0), _blendState(0),
_flags(mc._flags)
{
	for(int i=0;i<TS_COUNT;++i)
	{
		if (mc._textures[i].empty())
		{
			_textures[i] = MaterialSystem::getSingletonRef().getDefaultTexture((TextureSlot)i);
			_defaultTex[i] = true;
		}
		else
		{
			_textures[i] = device->createTexture(mc._textures[i]);
			_defaultTex[i] = false;
		}
	}

	_renderState = device->createRenderState(mc._cull);
	_blendState = device->createBlendState(mc._blendEnable, mc._srcBlend, mc._destBlend);

	if (mc._animations & MA_TEXTURE)
		_anims.getVector().push_back(new MaterialTextureAnimation(mc, this));

	if (mc._animations & MA_UV)
		_anims.getVector().push_back(new MaterialUVAnimation(mc, this));

	if (mc._animations & MA_FADE)
		_anims.getVector().push_back(new MaterialFadeAnimation(mc, this));
}

Material::~Material()
{
	_device->destroyBlendState(_blendState);
	_device->destroyRenderState(_renderState);

	for(int i=0;i<TS_COUNT;++i)
	{
		if(!_defaultTex[i])
			_textures[i]->remRef();
	}
}

void Material::setBase(float time)
{
	for(int i=0;i<=TS_SPECULAR;++i)
		_textures[i]->set(i);

	_device->setBlendState(_blendState);
	_device->setRenderState(_renderState);

	setTime(time);
}

void Material::setAdd(float time)
{
	_textures[TS_EMISSIVE]->set(0);
	_textures[TS_NORMALMAP]->set(1);
	_textures[TS_ENVIONMENT]->set(3);

//	_device->setBlendState(_blendState);
	_device->setRenderState(_renderState);

	setTime(time);
}

void Material::setShadow(float time)
{
	_textures[TS_DIFFUSE]->set(0);
	setTime(time);
}

void Material::setTime(float time)
{
	vector<MaterialAnimation*>::iterator a(_anims.getVector().begin());
	for(;a!=_anims.getVector().end();++a)
		(*a)->set(time);

	MaterialSystem::getSingletonRef().getBaseConstantBuffer()->fill((const void*)&_buffer);
}

void Material::createAnimation(const string& animName)
{
	_anims.getVector().push_back(MaterialAnimationFactory::createAnimation(animName,this));
}

void Material::deleteAnimation(int n)
{
	vector<MaterialAnimation*>::iterator a(_anims.getVector().begin()+n);
	delete *a;
	_anims.getVector().erase(a);
}

void Material::setTexture(TextureSlot s, const string& texFile)
{
	if(!_defaultTex[s])
	{
		_textures[s]->remRef();
		_textures[s]=MaterialSystem::getSingletonRef().getDefaultTexture(s);
		_defaultTex[s]=true;
	}

	if(texFile.length() > 0)
	{
		_textures[s]=_device->createTexture(texFile);
		_defaultTex[s]=false;
	}
}

void Material::updateStdBuffer()
{
	MaterialSystem::getSingletonRef().getBaseConstantBuffer()->fill((const void*)&_buffer);
}

bool Material::isAnimated() const
{
    return !_anims.getVector().empty();
}

int Material::getAnimationCount() const
{
    return _anims.getVector().size();
}

MaterialAnimation* Material::getAnimation(int n) const
{
    return _anims.getVector()[n];
}

void Material::addAnimation(MaterialAnimation* a)
{
    _anims.getVector().push_back(a);
}
