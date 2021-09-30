
#include "HDR.h"

const char *HDRShaderFiles[HDR::HDRS_COUNT] =
{
	"HDR/FirstGreyDS",
	"HDR/GreyDS",
	"HDR/Bright",
	"HDR/BrightDS",
	"HDR/HBloom",
	"HDR/VBloom",
	"HDR/Final"
};

HDR::HDR(IVideoDevice* d, const Config& cfg, IDepthTexture* depthBuffer) : _enable(true), _device(d), _accumulationBuffer(0), _accumulationFB(0),
    _HDRBackBuffer(0),_HDRBackFB(0), _cBuffer(0), _pointSampler(0), _linearSampler(0), _depthState(0), _blendState(0)

{
    for(int i=0;i<LUMINANCE_COUNT;++i)
        _luminanceFB[i]=0;

    for(int i=0;i<HDRT_COUNT;++i)
        _postFB[i]=0;

	_hdrBuffer._brightThreshold = 0.8f;
	_hdrBuffer._exposure = 0.7f;
	_hdrBuffer._gaussianScalar = 4.0f;

	cfg.getVar("HDR_enable", _enable);
	cfg.getVar("HDR_brightness_threshold", _hdrBuffer._brightThreshold);
	cfg.getVar("HDR_exposure", _hdrBuffer._exposure);
	cfg.getVar("HDR_gaussian", _hdrBuffer._gaussianScalar);

	for (int i = 0; i<LUMINANCE_COUNT; ++i)
		_luminanceTex[i] = 0;

	for (int i = 0; i<HDRS_COUNT; ++i)
		_shaders[i] = 0;

	for (int i = 0; i<HDRT_COUNT; ++i)
		_postTex[i] = 0;

	if (_enable)
	{
		_depthState = d->createDepthStencilState(false, false, COMP_ALWAYS);
		_blendState = d->createBlendState(false, BLEND_ONE, BLEND_ZERO);
		_pointSampler = d->createSamplerState(FILTER_POINT, ADDRESS_CLAMP, ADDRESS_CLAMP);
		_linearSampler = d->createSamplerState(FILTER_BILINEAR, ADDRESS_CLAMP, ADDRESS_CLAMP);

		for (int i = 0; i<HDRS_COUNT; ++i)
			_shaders[i] = d->createShader(HDRShaderFiles[i]);

		_cBuffer = d->createConstantBuffer(2, 4);
    }

    onResize(d->getResWidth(), d->getResHeight(),depthBuffer);
}

HDR::~HDR()
{
	if (_enable)
	{
		_device->destroySamplerState(_pointSampler);
		_device->destroySamplerState(_linearSampler);
		_device->destroyBlendState(_blendState);
		_device->destroyDepthStencilState(_depthState);

		delete _cBuffer;

		for (int i = 0; i < LUMINANCE_COUNT; ++i)
        {
			_luminanceTex[i]->remRef();
            delete _luminanceFB[i];
        }

		for (int i = 0; i < HDRS_COUNT; ++i)
			_shaders[i]->remRef();

		for (int i = 0; i < HDRT_COUNT; ++i)
        {
			_postTex[i]->remRef();
            delete _postFB[i];
        }
	}

    _HDRBackBuffer->remRef();
    _accumulationBuffer->remRef();

    delete _accumulationFB;
    delete _HDRBackFB;
}

void HDR::onResize(int w, int h, IDepthTexture* depthBuffer)
{
    TextureFormat fmt=TEXF_A8R8G8B8;

	if (_enable)
	{
        fmt=TEXF_A16R16G16B16F;

		const char* const LumTexNames[LUMINANCE_COUNT] = {
			"HDRLum01",
			"HDRLum02",
			"HDRLum03",
			"HDRLum04",
			"HDRLum05",
			"HDRLum06"
		};

		int texSize = 1;
		for (int i = 0; i < LUMINANCE_COUNT; ++i)
		{
            if (_luminanceTex[i])
            {
                _luminanceTex[i]->remRef();
                delete _luminanceFB[i];
                _luminanceFB[i]=0;
            }

			_luminanceTex[i] = _device->createTexture(LumTexNames[i], texSize, texSize, TEXF_G16R16F, BU_DEFAULT, true);
            _luminanceFB[i]=_device->createFrameBuffer(texSize,texSize,1,&_luminanceTex[i],0);

			texSize *= 3;
		}

		for (int i = 0; i < HDRT_COUNT; ++i)
		{
			if (_postTex[i])
            {
				_postTex[i]->remRef();
                delete _postFB[i];
                _postFB[i]=0;
            }
		}

		TextureFormat fmt = TEXF_A8R8G8B8;
		_postTex[HDRT_BRIGHT] = _device->createTexture("Bright", w / 2, h / 2, fmt, BU_DEFAULT, true);
		_postTex[HDRT_BRIGHTDS] = _device->createTexture("BrightDS", w / 8, h / 8, fmt, BU_DEFAULT, true);
		_postTex[HDRT_HBLOOM] = _device->createTexture("HBloom", w / 8, h / 8, fmt, BU_DEFAULT, true);
		_postTex[HDRT_VBLOOM] = _device->createTexture("VBloom", w / 8, h / 8, fmt, BU_DEFAULT, true);

        for(int i=0;i<HDRT_COUNT;++i)
            _postFB[i]=_device->createFrameBuffer(_postTex[i]->getWidth(),_postTex[i]->getHeight(),1,&_postTex[i],0);

		_hdrBuffer._invBloomTex.x = 8.0f / ((float)w);
		_hdrBuffer._invBloomTex.y = 8.0f / ((float)h);
		_hdrBuffer._greyScaleUV.x = ((float)w) / 243.0f;
		_hdrBuffer._greyScaleUV.y = ((float)h) / 243.0f;

        fmt=TEXF_A16R16G16B16F;
    }

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
    SamplerState ss[3] = { _pointSampler, _pointSampler, _linearSampler };
    _device->setSamplerState(0, 3, ss);

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

	//					Luminance

	//			First-step ds
    _luminanceFB[LUMINANCE_COUNT-1]->set();
	_HDRBackBuffer->set(0);
	_shaders[HDRS_FIRSTGREYDS]->set();
	_device->renderFullscreenQuad();

	//			DS to 1
	_shaders[HDRS_GREYDS]->set();
	for (int i = LUMINANCE_COUNT - 1; i>0; --i)
	{
        _luminanceFB[i-1]->set();
		_luminanceTex[i]->set(0);
		_device->renderFullscreenQuad();
	}

	//					Post-process

	//			Bright-pass
    _postFB[HDRT_BRIGHT]->set();
	_HDRBackBuffer->set(0);
	_shaders[HDRS_BRIGHT]->set();
	_device->renderFullscreenQuad();

	//			Bright DS
    _postFB[HDRT_BRIGHTDS]->set();
	_postTex[HDRT_BRIGHT]->set(0);
	_shaders[HDRS_BRIGHTDS]->set();
	_device->renderFullscreenQuad();

	//			H Bloom
    _postFB[HDRT_HBLOOM]->set();
	_HDRBackBuffer->set(0);
	_postTex[HDRT_BRIGHTDS]->set(0);
	_shaders[HDRS_HBLOOM]->set();
	_device->renderFullscreenQuad();

	//			V Bloom
    _postFB[HDRT_VBLOOM]->set();
	_HDRBackBuffer->set(0);
	_postTex[HDRT_HBLOOM]->set(0);
	_shaders[HDRS_VBLOOM]->set();
	_device->renderFullscreenQuad();

	//			Final
	_device->resetRenderTargets();
	_HDRBackBuffer->set(0);
	_luminanceTex[0]->set(1);
	_postTex[HDRT_VBLOOM]->set(2);
	_shaders[HDRS_FINAL]->set();
	_device->renderFullscreenQuad();
}
