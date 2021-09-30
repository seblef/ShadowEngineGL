
#include "GBuffer.h"



GBuffer::GBuffer(IVideoDevice* device) : _frameBuffer(0), _device(device), _diffuseRT(0), _normalRT(0), _depthRT(0)
{
	int w,h;
	device->getResolution(w,h);
	createResources(w,h);

	_DSState=device->createDepthStencilState(true,true,COMP_LESSEQUAL);
	_writeSState=device->createSamplerState(FILTER_TRILINEAR);
	_readSState=device->createSamplerState(FILTER_POINT,ADDRESS_CLAMP,ADDRESS_CLAMP);

	_copyBlendState=device->createBlendState(false,BLEND_ONE,BLEND_ZERO);
	_copyDepthState=device->createDepthStencilState(false,false,COMP_ALWAYS);
    _copyShader=device->createShader("Base/Copy");
	_copyCB=device->createConstantBuffer(1,0);
}

GBuffer::~GBuffer()
{
	_copyShader->remRef();
	delete _copyCB;
	_diffuseRT->remRef();
	_normalRT->remRef();
    delete _depthRT;

    delete _frameBuffer;

	_device->destroyDepthStencilState(_DSState);
	_device->destroySamplerState(_writeSState);
	_device->destroySamplerState(_readSState);

	_device->destroyBlendState(_copyBlendState);
	_device->destroyDepthStencilState(_copyDepthState);
}

void GBuffer::setAsRenderTarget() const
{
    _frameBuffer->set();

    _device->setDepthStencilState(_DSState);

	SamplerState ss[3]={ _writeSState, _writeSState, _writeSState};
	_device->setSamplerState(0,3,ss);
}

void GBuffer::setAsTextures() const
{
	_diffuseRT->set(0);
	_normalRT->set(1);
    _depthRT->set(2);

    SamplerState ss[3]={ _readSState, _readSState, _readSState };
    _device->setSamplerState(0,3,ss);
}

void GBuffer::copy(const Color& c) const
{
	_device->resetRenderTargets();
    setAsTextures();

	_copyShader->set();
    _copyCB->set();
	_copyCB->fill((const void*)&c);
	_device->setBlendState(_copyBlendState);
	_device->setDepthStencilState(_copyDepthState);
	_device->renderFullscreenQuad();
}

void GBuffer::copyShadowMap(IShadowMap *m) const
{
	_device->resetRenderTargets();
//	setAsTextures();

	m->set(0);
	_copyShader->set();
    _copyCB->set();
    _copyCB->fill((const void*)&Color::Red);
	_device->setBlendState(_copyBlendState);
	_device->setDepthStencilState(_copyDepthState);
	_device->renderFullscreenQuad();
}

void GBuffer::copyTexture(ITexture* t) const
{
	_device->resetRenderTargets();
//	setAsTextures();

	t->set(0);
	_copyShader->set();
    _copyCB->set();
	_copyCB->fill((const void*)&Color::Red);
	_device->setBlendState(_copyBlendState);
	_device->setDepthStencilState(_copyDepthState);
	_device->renderFullscreenQuad();
}

void GBuffer::clearResources()
{
	if(_diffuseRT)
	{
        delete _frameBuffer;
        _frameBuffer=0;

        _diffuseRT->remRef();
		_normalRT->remRef();
        delete _depthRT;

		_diffuseRT=_normalRT=0;
        _depthRT=0;
	}
}

void GBuffer::createResources(int w, int h)
{
	_diffuseRT=_device->createTexture("GAldebo",w,h,TEXF_A8R8G8B8,BU_DEFAULT,true);
	_normalRT=_device->createTexture("GNormal",w,h,TEXF_A8R8G8B8,BU_DEFAULT,true);
    _depthRT=_device->createDepthTexture(w,h,32,true);

    ITexture* rt[2]={_diffuseRT,_normalRT};
    _frameBuffer=_device->createFrameBuffer(w,h,2,rt,_depthRT);
}

void GBuffer::onResize(int w, int h)
{
	clearResources();
	createResources(w,h);
}
