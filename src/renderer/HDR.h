#pragma once

#include "GBuffer.h"
#include "../core/TSingleton.h"


namespace YAML
{
	class Node;
}

#define BLOOM_NUM_MIPS			5


class HDR : public TSingleton<HDR>
{
protected:

	struct HDRBuffer
	{
		Vector2 invScreenSize;
		float gamma;
		float exposure;
		float brightnessThreshold;
		float radius;
		float strength;
		float fpack;
	};

	bool				_enable;

	IVideoDevice*		_device;

	IShader*			_toneShader;

	ITexture*			_accumulationBuffer;
    IFrameBuffer*       _accumulationFB;

	ITexture*			_HDRBackBuffer;
    IFrameBuffer*       _HDRBackFB;
    IFrameBuffer* _renderingFB;

	ITexture*			_bloomSamples[BLOOM_NUM_MIPS];
	IFrameBuffer*		_bloomSamplesFB[BLOOM_NUM_MIPS];
	IShader* _bloomBright;
	IShader* _bloomDS;
	IShader* _bloomUS;

	IConstantBuffer*	_cBuffer;
	SamplerState		_pointSampler;
	SamplerState		_linearSampler;
	DepthStencilState	_depthState;
	BlendState			_blendState;
	BlendState _addBlendState;

	HDRBuffer			_hdrBuffer;

	void bloomPass();

public:

    HDR(IVideoDevice* d, const YAML::Node& cfg, IDepthTexture* depthBuffer);
	~HDR();

	ITexture*			getAccumulationBuffer()	const				{ return _accumulationBuffer; }
    IFrameBuffer*       getAccumulationFrameBuffer() const          { return _accumulationFB; }

	void				setBackBuffer();
    void				process(GBuffer* gbuf);

    void				onResize(int w, int h, IDepthTexture* depthBuffer);
    void setRenderingFrameBuffer(IFrameBuffer* frameBuffer);

	const ITexture* getBloomDownScale(unsigned int idx) const { return _bloomSamples[idx]; }
};
