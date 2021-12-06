
#include "HDAO.h"
#include "../core/YAMLCore.h"
#include <random>
#include <memory>


const unsigned int noiseTextureSize = 4;


ITexture *generateNoiseTexture(IVideoDevice* device)
{
	std::uniform_real_distribution<float> randomFloats(0.f, 1.f);
	std::default_random_engine generator;

	const unsigned int noiseCount = noiseTextureSize * noiseTextureSize;
	std::unique_ptr<Vector4[]> noise(new Vector4[noiseCount]);

	for(unsigned int i=0; i<noiseCount;++i)
		noise[i] = Vector4(
			randomFloats(generator) * 2.f - 1.f,
			randomFloats(generator) * 2.f - 1.f,
			0.f,
			0.f
		);

	return device->createTexture(
		"HDAONoise",
		noiseTextureSize,
		noiseTextureSize,
		TEXF_A32R32G32B32F,
		BU_DYNAMIC,
		false, false,
		(const void*)noise.get()
	);
}

void generateKernel(Vector4 *kernel, unsigned int kernelSize)
{
	std::uniform_real_distribution<float> randomFloats(0.f, 1.f);
	std::default_random_engine generator;

	for(unsigned int i=0; i<kernelSize; ++i)
	{
		Vector3 sample(
			randomFloats(generator) * 2.f - 1.f,
			randomFloats(generator) * 2.f - 1.f,
			randomFloats(generator)
		);
		sample.normalize();
		sample *= randomFloats(generator);

		float scale = float(i) / float(kernelSize);

		lerp<float>(0.1f, 1.f, scale * scale, scale);
		sample *= scale;

		kernel[i] = sample;
	}

	for(unsigned int i=kernelSize; i<HDAO_MAX_KERNEL_SIZE; ++i)
		kernel[i] = Vector4::NullVector;
}


HDAO::HDAO(IVideoDevice* d, const YAML::Node& cfg) :
	_enable(true),
	_device(d),
    _shader(0),
	_HDAOTexture(0),
	_HDAOFrameBuffer(0),
	_HDAOBuffer(0),
	_noiseTexture(0),
	_blendState(0),
	_gbufferSampler(0),
	_noiseSampler(0),
	_bufferScale(1)
{
	int w, h;
	d->getResolution(w, h);

	_hBuffer.kernelSize = 64;
	_hBuffer.radius = 0.5f;

	_enable = cfg["enable"].as<bool>(true);
	_hBuffer.radius = cfg["radius"].as<float>(_hBuffer.radius);
	_hBuffer.kernelSize = cfg["kernel_size"].as<float>(_hBuffer.kernelSize);

	if (_enable)
	{
		_shader = d->createShader("PostProcess/HDAO");
        _HDAOBuffer = d->createConstantBuffer(sizeof(HDAOBufffer_t) / (4 * sizeof(float)), 2);

		_blendState = d->createBlendState(false, BLEND_ONE, BLEND_ZERO);
		_gbufferSampler = d->createSamplerState(FILTER_POINT, ADDRESS_CLAMP, ADDRESS_CLAMP);
		_noiseSampler = d->createSamplerState(FILTER_POINT);
		_noiseTexture = generateNoiseTexture(d);
		generateKernel(_hBuffer.kernel, _hBuffer.kernelSize);

		onResize(w, h);
	}
	else
		_HDAOTexture = d->createTexture("Textures/Default/Diffuse.bmp");
}

HDAO::~HDAO()
{
	if (_shader)			_shader->remRef();
	if (_HDAOBuffer)		delete _HDAOBuffer;
	if (_HDAOTexture)		_HDAOTexture->remRef();
	if (_noiseTexture)		_noiseTexture->remRef();

	if (_blendState)		_device->destroyBlendState(_blendState);
	if (_gbufferSampler)	_device->destroySamplerState(_gbufferSampler);
	if (_noiseSampler)		_device->destroySamplerState(_noiseSampler);

    if(_HDAOFrameBuffer)        delete _HDAOFrameBuffer;
}

void HDAO::process(GBuffer& gbuf, Camera& c)
{
	if (!_enable)
		return;

	_shader->set();
    _HDAOFrameBuffer->set();

	_hBuffer.q = c.getZMax() / (c.getZMax() - c.getZMin());
	_hBuffer.qTimesNear = _hBuffer.q * c.getZMin();

    _HDAOBuffer->set();
	_HDAOBuffer->fill(&_hBuffer);

	_device->setBlendState(_blendState);

	SamplerState ss[4] = {
		_gbufferSampler,
		_gbufferSampler,
		_gbufferSampler,
		_noiseSampler
	};
	_device->setSamplerState(0, 4, ss);

	gbuf.setAsTextures();
	_noiseTexture->set(3);

	_device->renderFullscreenQuad();
}

void HDAO::onResize(int w, int h)
{
	if(!_enable)
		return;

    if(_HDAOFrameBuffer)        delete _HDAOFrameBuffer;
	if(_HDAOTexture)			_HDAOTexture->remRef();

	int bwidth = w / _bufferScale;
	int bheight = w / _bufferScale;
	_bufferSize = Vector2((float)bwidth, (float)bheight);

	_hBuffer.noiseScale = Vector2(
		(float)(bwidth / noiseTextureSize),
		(float)(bheight / noiseTextureSize)
	);

	_HDAOTexture = _device->createTexture("HDAO", bwidth, bheight, TEXF_R32F, BU_DEFAULT, true);
	ITexture* rt[1] = {_HDAOTexture};
	_HDAOFrameBuffer = _device->createFrameBuffer(bwidth, bheight, 1, rt, 0);
}
