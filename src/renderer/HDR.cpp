
#include "HDR.h"


HDR::HDR(IVideoDevice* d, const Config& cfg, IDepthTexture* depthBuffer) : _enable(true), _device(d), _accumulationBuffer(0), _accumulationFB(0),
    _HDRBackBuffer(0),_HDRBackFB(0), _cBuffer(0), _pointSampler(0), _linearSampler(0), _depthState(0), _blendState(0)

{
	_hdrBuffer.exposure = 0.7f;
	_hdrBuffer.gamma = 2.2f;

	cfg.getVar("HDR_enable", _enable);
	// cfg.getVar("HDR_brightness_threshold", _hdrBuffer._brightThreshold);
	cfg.getVar("HDR_exposure", _hdrBuffer.exposure);
	cfg.getVar("HDR_gamma", _hdrBuffer.gamma);

	if (_enable)
	{
		_depthState = d->createDepthStencilState(false, false, COMP_ALWAYS);
		_blendState = d->createBlendState(false, BLEND_ONE, BLEND_ZERO);
		_pointSampler = d->createSamplerState(FILTER_POINT, ADDRESS_CLAMP, ADDRESS_CLAMP);
		_linearSampler = d->createSamplerState(FILTER_BILINEAR, ADDRESS_CLAMP, ADDRESS_CLAMP);

		_toneShader = d->createShader("HDR/Tone");

		_cBuffer = d->createConstantBuffer(1, 4);
    }

    onResize(d->getResWidth(), d->getResHeight(),depthBuffer);
}

HDR::~HDR()
{
	if (_enable)
	{
		_toneShader->remRef();

		_device->destroySamplerState(_pointSampler);
		_device->destroySamplerState(_linearSampler);
		_device->destroyBlendState(_blendState);
		_device->destroyDepthStencilState(_depthState);

		delete _cBuffer;
	}

    _HDRBackBuffer->remRef();
    _accumulationBuffer->remRef();

    delete _accumulationFB;
    delete _HDRBackFB;
}

void HDR::onResize(int w, int h, IDepthTexture* depthBuffer)
{
	TextureFormat fmt;
	if(_enable)
		fmt = TEXF_A8R8G8B8;
	else
		fmt = TEXF_A16R16G16B16F;

    if (_HDRBackBuffer)
    {
        _HDRBackBuffer->remRef();
        delete _HDRBackFB;
        _HDRBackFB=0;
    }

    if (_accumulationBuffer)
    {
        _accumulationBuffer->remRef();
        delete _accumulationFB;
        _accumulationFB=0;
    }

    _HDRBackBuffer = _device->createTexture("HDRBack", w, h, fmt, BU_DEFAULT, true);
    _HDRBackFB=_device->createFrameBuffer(w,h,1,&_HDRBackBuffer,depthBuffer);

    _accumulationBuffer = _device->createTexture("LightAccumulation", w, h, fmt, BU_DEFAULT, true);
    _accumulationFB=_device->createFrameBuffer(w,h,1,&_accumulationBuffer,depthBuffer);
}

void HDR::setBackBuffer()
{
    _HDRBackFB->set();
}

void HDR::process(GBuffer* gbuf)
{
	_device->setSamplerState(0, _pointSampler);
    if (!_enable)
    {
        _device->resetRenderTargets();
        gbuf->copyTexture(_HDRBackBuffer);
		return;
    }

	_device->setDepthStencilState(_depthState);
	_device->setBlendState(_blendState);

    _cBuffer->set();
	_cBuffer->fill(&_hdrBuffer);

	// Tone mapping
	_toneShader->set();
	_device->resetRenderTargets();
	_HDRBackBuffer->set(0);
	_device->renderFullscreenQuad();
}
