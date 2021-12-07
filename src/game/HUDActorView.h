
#pragma once

#include "../core/TDynamicTab.h"
#include "../mediacommon/IVideoDevice.h"
#include <set>

namespace Core
{
    class Camera;
}

using namespace Core;

class IShader;
class IVertexBuffer;
class HUDActor;

class HUDActorView
{
protected:

	IVideoDevice*		_device;
	IShader*			_shader;
	IVertexBuffer*		_vBuffer;
	BlendState			_blendState;
	RenderState			_renderState;
	DepthStencilState	_dsState;
	TDynamicTab<VxBase>	_vertices;

public:

	HUDActorView(IVideoDevice* device);
	~HUDActorView();

	void				renderHUD(Camera& c, const set<HUDActor*>& actors);
};
