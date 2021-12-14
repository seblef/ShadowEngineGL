#pragma once

#include "../mediacommon/IVertexBuffer.h"
#include <memory>

class IVideoDevice;

namespace Core
{
    class Camera;
}

namespace Particles
{

class Particle;

class ParticleRenderer
{
	protected:

		IVideoDevice* _device;
		std::unique_ptr<IVertexBuffer>  _vBuffer;
		int _particleCount;

	public:

		ParticleRenderer(IVideoDevice *dev) : _device(dev), _particleCount(0) {}
		virtual ~ParticleRenderer() {}

		virtual const char* getRendererName() const { return "null"; }

		virtual void addParticle(Particle& p, const Core::Camera& c)=0;
		virtual void renderParticles()=0;
};

}