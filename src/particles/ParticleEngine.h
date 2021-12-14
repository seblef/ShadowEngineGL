#pragma once

#include "ParticleSystem.h"
#include "RendererPoint.h"
#include "RendererQuad.h"
#include "../core/TSingleton.h"

class IVideoDevice;
class IShader;

class ParticleEngine : public TSingleton<ParticleEngine>
{
	protected:

		IVideoDevice*				_device;
		IShader*					_stdShader;
		DepthStencilState			_dsState;
		RenderState					_rsState;
		SamplerState				_samplerState;

		RendererPoint					_pointRenderer;
		RendererQuad						_quadRenderer;
		
		Particle**					_particles;

		unsigned int				_maxParticles;
		unsigned int				_particlesOnScreen;

		typedef vector<ParticleSystem*>		PSVector;
		PSVector					_queue;

	public:

		ParticleEngine(unsigned int maxParticles, IVideoDevice *d);
		~ParticleEngine();

		unsigned int				getMaxParticles() const				{ return _maxParticles; }
		unsigned int				getParticlesOnScreen() const		{ return _particlesOnScreen; }

		void						enqueueSystem(ParticleSystem *s)	{ _queue.push_back(s); }

		void						draw(const Camera& c);

		RendererPoint*					getPointRenderer()					{ return &_pointRenderer; }
		RendererQuad*					getQuadRenderer()					{ return &_quadRenderer; }
		ParticleRenderer*			getRenderer(const string& r);

		IVideoDevice*				getVideoDevice() const				{ return _device; }
};
