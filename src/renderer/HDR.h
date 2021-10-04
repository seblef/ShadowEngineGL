#pragma once

#include "../MediaCommon.h"
#include "GBuffer.h"


class HDR : public TSingleton<HDR>
{
protected:

	struct HDRBuffer
	{
		float gamma;
		float exposure;
		Vector2 f2pack;
	};

	bool				_enable;

	IVideoDevice*		_device;

	IShader*			_toneShader;

	ITexture*			_accumulationBuffer;
    IFrameBuffer*       _accumulationFB;

	ITexture*			_HDRBackBuffer;
    IFrameBuffer*       _HDRBackFB;

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
