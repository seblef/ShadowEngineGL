
#include "MaterialSystem.h"
#include "../mediacommon/IConstantBuffer.h"
#include "../mediacommon/ITexture.h"



const char* const g_MaterialDefaultTexs[TS_COUNT]={
	"Textures/Default/Diffuse.bmp",
	"Textures/Default/Normal.bmp",
	"Textures/Default/Specular.bmp",
	"Textures/Default/Emissive.bmp",
    "Textures/Default/Environment.bmp"
};

MaterialSystem::MaterialSystem(IVideoDevice* device) : _device(device)
{
	for(int i=0;i<TS_COUNT;++i)
		_defaultTexs[i]=device->createTexture(g_MaterialDefaultTexs[i]);

	_samplerState=device->createSamplerState(FILTER_TRILINEAR);
	_baseCB=device->createConstantBuffer(6,2);
}

MaterialSystem::~MaterialSystem()
{
	_device->destroySamplerState(_samplerState);
	delete _baseCB;

	for(int i=0;i<TS_COUNT;++i)
		_defaultTexs[i]->remRef();
}

void MaterialSystem::setBuffers() const
{
    _baseCB->set();

	SamplerState ss[4]={
		_samplerState,
		_samplerState,
		_samplerState,
		_samplerState
	};

	_device->setSamplerState(0,4,ss);
}
