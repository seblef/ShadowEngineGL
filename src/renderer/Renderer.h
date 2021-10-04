#pragma once

#include "../MediaCommon.h"
#include "GroundData.h"
#include "SceneInfosBuffer.h"
#include "GBuffer.h"
#include "IVisibilitySystem.h"
#include "BufferView.h"

class Renderer : public TSingleton<Renderer>
{
protected:

	enum BufferViewFlag {
		BVF_NONE = 0,
		BVF_GBUFFER = 1,
		BVF_ACCUMULATION = 2,
		BVF_GLOBALSHADOWMAP = 4,
		BVF_LOCALSHADOWMAP = 8,
		BVF_HDR = 16,
		BVF_HDAO = 32
	};

	IVideoDevice*		_device;
	IConstantBuffer*	_sceneInfosCS;
	IConstantBuffer*	_objectInfosCS;
	DepthStencilState	_dsSolidWrite;
	DepthStencilState	_dsSolidAdd;
	BlendState			_bsAdd;

	SceneInfosBuffer	_sceneInfos;
	GBuffer				_GBuffer;
	BufferView			_bufferView;
	int					_bvFlags;

	GroundData*			_g_data;

	SRect				_visBounds;
	IVisibilitySystem*	_visAlways;
	IVisibilitySystem*	_vis;

	Camera*				_currentCamera;

	void				computeVisBounds(const ViewFrustum& vf, int w, int h);
	void				visibility(const Camera& c);
	void				renderPass(bool add);
	void 				parseBufferViewFlags(Config& cfg);
	void				renderBufferViews();

public:

	Renderer(IVideoDevice *device, Config& cfg);
	~Renderer();

	IVideoDevice*		getVideoDevice() const											{ return _device; }

	void				onResize(int w, int h);

	void				setGroundData(GroundData* data)									{ _g_data=data; }

	void				update(float time, Camera *c);
	void				renderFlags(unsigned int f, const unsigned int* flagMap, int w) const;

	void				setCurrentCamera(Camera *c);
	Camera				*getCurrentCamera() const										{ return _currentCamera; }

	void				setObjectTransformation(const Matrix4& trans) const				{ _objectInfosCS->fill((const void*)&trans); }
	void				setSceneInfos() const;

	void				addRenderable(Renderable* r);
	void				remRenderable(Renderable* r);

	IVisibilitySystem*	getVisibilitySystem() const										{ return _vis; }
	const SRect&		getVisibilityBounds() const										{ return _visBounds; }
};
