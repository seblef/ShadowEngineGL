#pragma once

#include "Material.h"

class MaterialSystem : public TSingleton<MaterialSystem>
{
protected:

	IVideoDevice*		_device;
	ITexture*			_defaultTexs[TS_COUNT];
	IConstantBuffer*	_baseCB;
	SamplerState		_samplerState;

public:

	MaterialSystem(IVideoDevice *device);
	~MaterialSystem();

	ITexture*			getDefaultTexture(TextureSlot ts) const						{ return _defaultTexs[ts]; }
	IConstantBuffer*	getBaseConstantBuffer() const								{ return _baseCB; }

	void				setBuffers() const;
};
