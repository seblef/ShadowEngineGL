
#ifndef _HDR_H_
#define _HDR_H_

#include "../MediaCommon.h"
#include "GBuffer.h"

#define LUMINANCE_COUNT				6

class HDR : public TSingleton<HDR>
{
public:

	enum HDRShaders
	{
		HDRS_FIRSTGREYDS = 0,
		HDRS_GREYDS,
		HDRS_BRIGHT,
		HDRS_BRIGHTDS,
		HDRS_HBLOOM,
		HDRS_VBLOOM,
		HDRS_FINAL,

		HDRS_COUNT
	};

	enum HDRTextures
	{
		HDRT_BRIGHT = 0,
		HDRT_BRIGHTDS,
		HDRT_HBLOOM,
		HDRT_VBLOOM,

		HDRT_COUNT
	};


protected:

	struct HDRBuffer
	{
		Vector2		_invBloomTex;
		Vector2		_greyScaleUV;
		float		_brightThreshold;
		float		_exposure;
		float		_gaussianScalar;
		float		f3BrightPack;
	};

	bool				_enable;

	IVideoDevice*		_device;

	ITexture*			_accumulationBuffer;
    IFrameBuffer*       _accumulationFB;

	ITexture*			_HDRBackBuffer;
    IFrameBuffer*       _HDRBackFB;

	ITexture*			_luminanceTex[LUMINANCE_COUNT];
    IFrameBuffer*       _luminanceFB[LUMINANCE_COUNT];

	ITexture*			_postTex[HDRT_COUNT];
    IFrameBuffer*       _postFB[HDRT_COUNT];

	IShader*			_shaders[HDRS_COUNT];
	IConstantBuffer*	_cBuffer;
	SamplerState		_pointSampler;
	SamplerState		_linearSampler;
	DepthStencilState	_depthState;
	BlendState			_blendState;

	HDRBuffer			_hdrBuffer;

public:

    HDR(IVideoDevice* d, const Config& cfg, IDepthTexture* depthBuffer);
	~HDR();

	ITexture*			getAccumulationBuffer()	const				{ return _accumulationBuffer; }
    IFrameBuffer*       getAccumulationFrameBuffer() const          { return _accumulationFB; }

	void				setBackBuffer();
    void				process(GBuffer* gbuf);

    void				onResize(int w, int h, IDepthTexture* depthBuffer);
};

#endif
