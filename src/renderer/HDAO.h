#pragma once

#include "GBuffer.h"
#include "../core/TSingleton.h"


#define HDAO_MAX_KERNEL_SIZE			64

namespace YAML
{
	class Node;
}

namespace Core
{
    class Camera;
}

using namespace Core;


class HDAO : public TSingleton<HDAO>
{
protected:

	bool				_enable;

	struct HDAOBufffer_t
	{
		Vector4 kernel[HDAO_MAX_KERNEL_SIZE];
		Vector2 noiseScale;
		float q;
		float qTimesNear;
		float radius;
		int kernelSize;
		Vector2 f2pack;
	};

	HDAOBufffer_t		_hBuffer;

	IVideoDevice*		_device;
	IShader*			_shader;
	ITexture*			_HDAOTexture;
    IFrameBuffer*       _HDAOFrameBuffer;
	ITexture* _noiseTexture;
	IConstantBuffer*	_HDAOBuffer;
	BlendState			_blendState;
	SamplerState		_gbufferSampler;
	SamplerState		_noiseSampler;
	int 				_bufferScale;
	Vector2				_bufferSize;

public:

	HDAO(IVideoDevice *d, const YAML::Node& cfg);
	~HDAO();

	bool				isEnabled() const { return _enable; }

	void				process(GBuffer& gbuf, Camera& cam);
	void				onResize(int w, int h);

	ITexture*			getHDAOTexture() const					{ return _HDAOTexture; }
	ITexture*			getNoiseTexture() const					{ return _noiseTexture; }
};
