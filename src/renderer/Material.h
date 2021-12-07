#pragma once

#include "MaterialCreate.h"
#include "MaterialStdBuffer.h"
#include "../core/TVector.h"

class MaterialAnimation;

class Material
{
protected:

	TVector<MaterialAnimation>	_anims;

	IVideoDevice*				_device;
	MaterialStdBuffer			_buffer;
	ITexture*					_textures[TS_COUNT];
	bool						_defaultTex[TS_COUNT];
	RenderState					_renderState;
	BlendState					_blendState;
	unsigned int				_flags;

public:

	Material(IVideoDevice *d);
	Material(const MaterialCreate& mc, IVideoDevice* d);
	virtual ~Material();

	unsigned int				getFlag(unsigned int f) const				{ return _flags & f; }
	void						setFlag(unsigned int f)						{ _flags|=f; }
	void						unsetFlag(unsigned int f)					{ _flags&= ~f; }

	virtual void				setBase(float time);
	virtual void				setAdd(float time);
	virtual void				setShadow(float time);
	virtual void				setTime(float time);

	MaterialStdBuffer&			getStdBuffer()								{ return _buffer; }
	const MaterialStdBuffer&	getStdBuffer() const						{ return _buffer; }

	bool						isAnimated() const;
	int							getAnimationCount() const;
	MaterialAnimation*			getAnimation(int n)	const;
	void						createAnimation(const string& animName);
	void						addAnimation(MaterialAnimation* a);
	void						deleteAnimation(int n);

	bool						isDefaultTexture(TextureSlot s) const		{ return _defaultTex[s]; }
	ITexture*					getTexture(TextureSlot s) const				{ return _textures[s] ;}
	void						setTexture(TextureSlot s, const string& texFile);

	BlendState					getBlendState() const						{ return _blendState; }
	RenderState					getRenderState() const						{ return _renderState; }

	void						setBlendState(BlendState bs)				{ _blendState=bs; }
	void						setRenderState(RenderState rs)				{ _renderState=rs; }

	void						updateStdBuffer();
};
