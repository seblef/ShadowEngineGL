#pragma once

#include "ShadowMapsBase.h"
#include "../core/TSingleton.h"
#include "../mediacommon/IVideoDevice.h"
#include <vector>

class Light;
class LightGlobal;

namespace Core
{
    class Camera;
}

using namespace Core;

class ShadowSystem : public TSingleton<ShadowSystem>
{
protected:

	typedef std::vector<Light*>		LightVector;

	IVideoDevice*				_device;
	ShadowMapsBase				_base;

	BlendState					_blendState;
	RenderState					_renderState;
	DepthStencilState			_dsState;

	LightVector					_lights;
	LightGlobal*				_global;

	void						getSpotLightCamera(Light* l, Camera& c) const;
	void						getAreaLightCamera(Light* l, Camera& c) const;
	void						render(IShadowMap* m, Camera& c) const;

public:

	ShadowSystem(IVideoDevice* device);
	~ShadowSystem();

	void						setGlobalLight(LightGlobal* g)	{ _global=g; }
	LightGlobal*				getGlobalLight() const { return _global; }

	void						enqueueLight(Light *l);
	void						renderShadowMaps();
	void						endRender();

	unsigned int 				getNumLights() const;
	Light*						getLight(int n);
	IShadowMap*					getShadowMap(ShadowMapsBase::MapSize size, int num) const;

    void						getGlobalLightCamera(Camera& c) const;
};
