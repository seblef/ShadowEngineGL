
#include "HDR.h"
#include "../loguru.hpp"


HDR::HDR(
	IVideoDevice* d,
	const Config& cfg,
	IDepthTexture* depthBuffer
) :
	_enable(true),
	_device(d),
	_accumulationBuffer(0),
	_accumulationFB(0),
    _HDRBackBuffer(0),
	_HDRBackFB(0),
	_cBuffer(0),
	_pointSampler(0),
	_linearSampler(0),
	_depthState(0),
	_blendState(0),
	_addBlendState(0),
	_bloomBright(0),
	_bloomDS(0),
	_bloomUS(0)
{
	_hdrBuffer.exposure = 0.7f;
	_hdrBuffer.gamma = 2.2f;

	cfg.getVar("HDR_enable", _enable);
	cfg.getVar("HDR_brightness_threshold", _hdrBuffer.brightnessThreshold);
	cfg.getVar("HDR_bloom_radius", _hdrBuffer.radius);
	cfg.getVar("HDR_bloom_strength", _hdrBuffer.strength);
	cfg.getVar("HDR_exposure", _hdrBuffer.exposure);
	cfg.getVar("HDR_gamma", _hdrBuffer.gamma);

	for(unsigned int i=0;i<BLOOM_NUM_MIPS;++i)
	{
		_bloomSamples[i] = 0;
		_bloomSamplesFB[i] = 0;
	}

	if (_enable)
	{
		_depthState = d->createDepthStencilState(false, false, COMP_ALWAYS);
		_blendState = d->createBlendState(false, BLEND_ONE, BLEND_ZERO);
		_addBlendState = d->createBlendState(true, BLEND_ONE, BLEND_ONE);
		_pointSampler = d->createSamplerState(FILTER_POINT, ADDRESS_CLAMP, ADDRESS_CLAMP);
		_linearSampler = d->createSamplerState(FILTER_BILINEAR, ADDRESS_CLAMP, ADDRESS_CLAMP);

		_toneShader = d->createShader("HDR/Tone");
		_bloomBright = d->createShader("HDR/BloomBright");
		_bloomDS = d->createShader("HDR/BloomDS");
		_bloomUS = d->createShader("HDR/BloomUS");

		_cBuffer = d->createConstantBuffer(2, 4);
    }

    onResize(d->getResWidth(), d->getResHeight(),depthBuffer);
}

HDR::~HDR()
{
	if (_enable)
	{
		_toneShader->remRef();
		_bloomBright->remRef();
		_bloomDS->remRef();
		_bloomUS->remRef();

		_device->destroySamplerState(_pointSampler);
		_device->destroySamplerState(_linearSampler);
		_device->destroyBlendState(_blendState);
		_device->destroyBlendState(_addBlendState);
		_device->destroyDepthStencilState(_depthState);

		delete _cBuffer;
	}

	for(unsigned int i=0;i<BLOOM_NUM_MIPS;++i)
	{
		if(_bloomSamplesFB[i])
		{
			_bloomSamples[i]->remRef();
			delete _bloomSamplesFB[i];
		}
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

	if(!_enable)
		return;

	int width = w, height = h;
	char name[50];
	for(unsigned int i=0;i<BLOOM_NUM_MIPS;++i)
	{
		snprintf(name, sizeof(name), "BloomSample%i", i);
		_bloomSamples[i] = _device->createTexture(name, width, height, TEXF_G11R11B10F, BU_DEFAULT, true);
		_bloomSamplesFB[i] = _device->createFrameBuffer(width, height, 1, &_bloomSamples[i], 0);

		width /= 2;
		height /= 2;
	}
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

    _cBuffer->set();
	_cBuffer->fill(&_hdrBuffer);

	bloomPass();

	// Tone mapping
	_device->setBlendState(_blendState);
	_toneShader->set();
	_device->resetRenderTargets();
	_HDRBackBuffer->set(0);
	_bloomSamples[0]->set(1);
	_device->renderFullscreenQuad();
}

void HDR::bloomPass()
{
	// Bright & Downscale
	_device->setSamplerState(0, _linearSampler);
	_device->setBlendState(_blendState);
	_bloomBright->set();
	_HDRBackBuffer->set(0);
	_bloomSamplesFB[0]->set();
	_device->renderFullscreenQuad();

	// Downscale
	_bloomDS->set();
	for(unsigned int i=1;i<BLOOM_NUM_MIPS;++i)
	{
		_hdrBuffer.invScreenSize.x = 1.f / (float)_bloomSamples[i]->getWidth();
		_hdrBuffer.invScreenSize.y = 1.f / (float)_bloomSamples[i]->getHeight();
		_cBuffer->fill(&_hdrBuffer);

		_bloomSamples[i-1]->set(0);
		_bloomSamplesFB[i]->set();
		_device->renderFullscreenQuad();
	}

	// // Upscale
	_device->setBlendState(_addBlendState);
	_bloomUS->set();

	for(int i=BLOOM_NUM_MIPS-2; i>=0; --i)
	{
		_hdrBuffer.invScreenSize.x = 1.f / (float)_bloomSamples[i]->getWidth();
		_hdrBuffer.invScreenSize.y = 1.f / (float)_bloomSamples[i]->getHeight();
		_cBuffer->fill(&_hdrBuffer);

		_bloomSamples[i+1]->set(0);
		_bloomSamplesFB[i]->set();
		_device->renderFullscreenQuad();
	}
}