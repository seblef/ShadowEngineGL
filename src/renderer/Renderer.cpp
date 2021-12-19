
#include "Actor.h"
#include "Mesh.h"
#include "Renderer.h"
#include "RendererStats.h"
#include "GroundRenderer.h"
#include "MaterialSystem.h"
#include "MeshRenderer.h"
#include "VisibilityFactory.h"
#include "LightSystem.h"
#include "UpdateSystem.h"
#include "ActorRenderer.h"
#include "ActorSkinnedRenderer.h"
#include "ShadowSystem.h"
#include "HDAO.h"
#include "HDR.h"
#include "../loguru.hpp"
#include "../core/CoreCommon.h"
#include "../core/Camera.h"
#include "../core/ViewFrustum.h"
#include "../core/YAMLCore.h"
#include "../mediacommon/IFrameBuffer.h"
#include "../particles/Engine.h"

// #define RENDERER_DEBUG


extern Light*   g_ShadowLight;



void SceneInfosBuffer::setCamera(const Camera& c)
{
    _viewProj=c.getViewProjMatrix();
    _proj=c.getProjectionMatrix();
    _viewProj.inverse(_invViewProj);
    _view = c.getViewMatrix();
    _proj.inverse(_invProj);
    _eyePos=c.getPosition();
    _invScreenSize=Vector2(0.5f / c.getCenter().x,0.5f / c.getCenter().y);
}



Renderer::Renderer(IVideoDevice *device, const YAML::Node& cfg) :
	_device(device),
    _frameBuffer(0),
    _g_data(0),
	_GBuffer(device),
	_bufferView(device, 6),
	_currentCamera(0),
	_bvFlags(BVF_NONE)
{
    YAML::Node hdr_cfg = cfg["hdr"];
    YAML::Node hdao_cfg = cfg["hdao"];

	new UpdateSystem;
	new GroundRenderer(device);
	new MaterialSystem(device);
	new LightSystem(device);
	new MeshRenderer(device);
	new ActorRenderer(device);
	new ActorSkinnedRenderer(device);
	new ShadowSystem(device);
	new Particles::Engine(10000, device);
	new HDAO(device, hdao_cfg);
    new HDR(device, hdr_cfg, _GBuffer.getDepthBuffer());

	_sceneInfosCS=device->createConstantBuffer(sizeof(SceneInfosBuffer) / (4*sizeof(float)),0);
	_objectInfosCS=device->createConstantBuffer(4,1);

	_visAlways=VisibilityFactory::createVisibilitySystem("all");
	_vis=VisibilityFactory::createVisibilitySystem("quad");

	_dsSolidWrite=device->createDepthStencilStateEx(true,true,COMP_LESSEQUAL,
		true,0xff,0xff,
		COMP_ALWAYS, STENCIL_KEEP, STENCIL_KEEP, STENCIL_REPLACE,
		COMP_ALWAYS, STENCIL_KEEP, STENCIL_KEEP, STENCIL_REPLACE);
	_dsSolidAdd = device->createDepthStencilState(true, false, COMP_EQUAL2);

	_bsAdd = device->createBlendState(true, BLEND_ONE, BLEND_ONE);

	parseBufferViewFlags(cfg);
}

Renderer::~Renderer()
{
	Particles::Engine::deleteSingleton();
	GroundRenderer::deleteSingleton();
	MaterialSystem::deleteSingleton();
	LightSystem::deleteSingleton();
	MeshRenderer::deleteSingleton();
	ActorRenderer::deleteSingleton();
	ActorSkinnedRenderer::deleteSingleton();
	UpdateSystem::deleteSingleton();
	ShadowSystem::deleteSingleton();
	HDAO::deleteSingleton();
	HDR::deleteSingleton();

	if(_g_data)			delete _g_data;

	delete _objectInfosCS;
	delete _sceneInfosCS;

	delete _visAlways;
	delete _vis;

	_device->destroyDepthStencilState(_dsSolidWrite);
	_device->destroyDepthStencilState(_dsSolidAdd);
	_device->destroyBlendState(_bsAdd);
}

void Renderer::setCurrentCamera(Camera *c)
{
	c->buildMatrices();
	_sceneInfos.setCamera(*c);
	_sceneInfosCS->fill((const void*)&_sceneInfos);
	_currentCamera=c;
}

void Renderer::update(float time, Camera *c)
{
#ifdef _OUTPUTSTATS
	RendererStats::getSingletonRef().newFrame();
#endif

#ifdef RENDERER_DEBUG
    LOG_S(2) << "- Renderer::updating updatables...";
#endif

    UpdateSystem::getSingletonRef().process(time);

#ifdef RENDERER_DEBUG
    LOG_S(2) << "- Renderer::computing visibility...";
#endif

    setCurrentCamera(c);
	visibility(*c);

#ifdef RENDERER_DEBUG
    LOG_S(2) << "- Renderer::preparing renderers...";
#endif

	MeshRenderer::getSingletonRef().prepareRender();
	ActorRenderer::getSingletonRef().prepareRender();
	ActorSkinnedRenderer::getSingletonRef().prepareRender();

//	_device->newFrame();
	_device->resetRenderTargets();

    _sceneInfosCS->set();
    _objectInfosCS->set();

#ifdef RENDERER_DEBUG
    LOG_S(2) << "- Renderer::rendering shadowmaps...";
#endif

    ShadowSystem::getSingletonRef().renderShadowMaps();
	_device->resetRenderTargets();

#ifdef RENDERER_DEBUG
    LOG_S(2) << "- Renderer::gbuffer pass...";
#endif

    _GBuffer.setAsRenderTarget();

	_device->clearDepthStencil();
    _device->clearRenderTargets(Color::Black);

#ifdef RENDERER_DEBUG
    LOG_S(2) << " - Setting material buffers...";
#endif

    MaterialSystem::getSingletonRef().setBuffers();

#ifdef RENDERER_DEBUG
    LOG_S(2) << " - Rendering...";
#endif

    renderPass(false);

#ifdef RENDERER_DEBUG
    LOG_S(2) << "- Renderer::HDAO pass...";
#endif

    HDAO::getSingletonRef().process(_GBuffer, *c);

	_device->resetRenderTargets();

#ifdef RENDERER_DEBUG
    LOG_S(2) << "- Renderer::lighting pass...";
#endif

    LightSystem::getSingletonRef().applyLights(*c,_GBuffer);

#ifdef RENDERER_DEBUG
    LOG_S(2) << "- Renderer::add material pass...";
#endif

    HDR::getSingletonRef().setBackBuffer();

    _sceneInfosCS->set();
    _objectInfosCS->set();

//    MaterialSystem::getSingletonRef().setBuffers();
//	renderPass(true);

#ifdef RENDERER_DEBUG
    LOG_S(2) << "- Renderer::closing renderers...";
#endif

    MeshRenderer::getSingletonRef().endRender();
	ActorRenderer::getSingletonRef().endRender();
	ActorSkinnedRenderer::getSingletonRef().endRender();

#ifdef RENDERER_DEBUG
    LOG_S(2) << "- Renderer::rendering particles...";
#endif

    Particles::Engine::getSingletonRef().draw(*_currentCamera);

#ifdef RENDERER_DEBUG
    LOG_S(2) << "- Renderer::HDR pass...";
#endif

    HDR::getSingletonRef().process(&_GBuffer);
    if(_frameBuffer)
        _frameBuffer->set();
    else
        _device->resetRenderTargets();

	renderBufferViews();
    ShadowSystem::getSingletonRef().endRender();
}

void Renderer::renderPass(bool add)
{
	_device->setPrimitiveType(PT_TRIANGLELIST);
	_device->setDepthStencilState(add ? _dsSolidAdd : _dsSolidWrite);
	_device->setStencilRef(1);

	if (add)
		_device->setBlendState(_bsAdd);

	ActorSkinnedRenderer::getSingletonRef().render(add);
	ActorRenderer::getSingletonRef().render(add);
	MeshRenderer::getSingletonRef().render(add);

	if(_g_data)
	{
		setObjectTransformation(Matrix4::Identity);
		GroundRenderer::getSingletonRef().renderGround(*_g_data,add);
	}
}

void Renderer::visibility(const Camera& c)
{
	ViewFrustum vf(c.getViewProjMatrix());
	vf.setPosition(c.getPosition());

	int w=65535,h=65535;
	if(_g_data)
	{
		w=_g_data->getWidth();
		h=_g_data->getHeight();
	}

	computeVisBounds(vf,w,h);

	if(_g_data)
		GroundRenderer::getSingletonRef().visibility(*_g_data,_visBounds);

	_visAlways->visibility(vf,_visBounds);
	_vis->visibility(vf,_visBounds);

}

void Renderer::computeVisBounds(const ViewFrustum& vf, int w, int h)
{
	Vector3 corners[5];
	Vector3 d[2];
	float _xmin,_xmax,_ymin,_ymax;
	int c_count=5;

	corners[0]=vf.getFarLeftDown();
	corners[1]=vf.getFarLeftUp();
	corners[2]=vf.getFarRightDown();
	corners[3]=vf.getFarRightUp();
	corners[4]=vf.getPosition();

	d[0]=corners[1] - vf.getPosition();
	d[1]=corners[3] - vf.getPosition();

	if(d[0].y < 0.0f && d[1].y < 0.0f)
	{
		c_count=4;
		Plane p(Vector3::NullVector,Vector3::YAxisVector);
		ViewFrustumPlane vfp[4][2]={
			{ VFP_BOTTOM, VFP_LEFT },
			{ VFP_LEFT, VFP_TOP },
			{ VFP_TOP, VFP_RIGHT },
			{ VFP_RIGHT, VFP_BOTTOM }};

		for(int i=0;i<4;++i)
			p.intersectWithPlanes(vf.getPlane(vfp[i][0]),vf.getPlane(vfp[i][1]),corners[i]);
	}

	_xmin=_xmax=corners[0].x;
	_ymin=_ymax=corners[0].z;

	for(int i=1;i<c_count;++i)
	{
		_xmin=smin(_xmin,corners[i].x);
		_xmax=smax(_xmax,corners[i].x);

		_ymin=smin(_ymin,corners[i].z);
		_ymax=smax(_ymax,corners[i].z);
	}

	_visBounds.x1=smax(((int)floorf(_xmin))-1,0);
	_visBounds.y1=smax(((int)floorf(_ymin))-1,0);

	_visBounds.x2=smin(((int)ceilf(_xmax))+1,w-1);
	_visBounds.y2=smin(((int)ceilf(_ymax))+1,h-1);
}

void Renderer::setSceneInfos() const
{
	_sceneInfosCS->set();
}

void Renderer::addRenderable(Renderable* r)
{
	if(r->isAlwaysVisible())
		_visAlways->addRenderable(r);
	else
		_vis->addRenderable(r);
}

void Renderer::remRenderable(Renderable* r)
{
	if(r->isAlwaysVisible())
		_visAlways->remRenderable(r);
	else
		_vis->remRenderable(r);
}

void Renderer::renderFlags(unsigned int f, const unsigned int* flagMap, int w) const
{
	_device->resetRenderTargets();
	GroundRenderer::getSingletonRef().renderFlag(f,flagMap,w);
}

void Renderer::onResize(int w, int h)
{
	_GBuffer.onResize(w, h);
	HDAO::getSingletonRef().onResize(w, h);
    HDR::getSingletonRef().onResize(w, h, _GBuffer.getDepthBuffer());
}

void Renderer::parseBufferViewFlags(const YAML::Node& cfg)
{
	const map<string, Renderer::BufferViewFlag> flagsMap = {
		{"gbuffer", BVF_GBUFFER},
		{"accumulation", BVF_ACCUMULATION},
		{"gshadow", BVF_GLOBALSHADOWMAP},
		{"lshadow", BVF_LOCALSHADOWMAP},
		{"hdr", BVF_HDR},
		{"hdao", BVF_HDAO}
	};

	const string& views(cfg["buffer_views"].as<string>(""));
	for(auto const& flag : flagsMap)
	{
		if(views.find(flag.first) != string::npos)
			_bvFlags |= flag.second;
	}
}

void Renderer::renderBufferViews()
{
	if(_bvFlags == BVF_NONE)
		return;

	if(_bvFlags & BVF_GBUFFER)
	{
		_bufferView.addTexture(_GBuffer.getDiffuseTex());
		_bufferView.addTexture(_GBuffer.getNormalTex());
		_bufferView.addDepth(_GBuffer.getDepthBuffer());
	}

	if(_bvFlags & BVF_ACCUMULATION)
		_bufferView.addTexture(HDR::getSingletonRef().getAccumulationBuffer());
	
	if(_bvFlags & BVF_GLOBALSHADOWMAP)
		_bufferView.addShadow(ShadowSystem::getSingletonRef().getGlobalLight()->getShadowMap());

	if(_bvFlags & BVF_LOCALSHADOWMAP)
	{
		for(int i=0;i<ShadowSystem::getSingletonRef().getNumLights();++i)
			_bufferView.addShadow(ShadowSystem::getSingletonRef().getLight(i)->getShadowMap());
	}

	if(_bvFlags & BVF_HDR)
	{
		for(unsigned int i=0;i<BLOOM_NUM_MIPS;++i)
			_bufferView.addTexture(HDR::getSingletonRef().getBloomDownScale(i));
	}

	if(_bvFlags & BVF_HDAO)
	{
		_bufferView.addTexture(HDAO::getSingletonRef().getHDAOTexture());
	}

	_bufferView.render();
}

void Renderer::setFrameBuffer(IFrameBuffer* frameBuffer, bool resize)
{
    _frameBuffer = frameBuffer;
    if(resize)
    {
        if(frameBuffer)
            onResize(frameBuffer->getWidth(), frameBuffer->getHeight());
        else
            onResize(_device->getResWidth(), _device->getResHeight());
    }

    HDR::getSingletonRef().setRenderingFrameBuffer(frameBuffer);
}