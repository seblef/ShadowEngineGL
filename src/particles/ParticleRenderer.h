#pragma once


class IVideoDevice;
class IVertexBuffer;
class Particle;

namespace Core
{
    class Camera;
}

using namespace Core;

class ParticleRenderer
{
	protected:

		IVideoDevice*			_device;
		IVertexBuffer*			_vBuffer;
		int						_particleCount;

	public:

		ParticleRenderer(IVideoDevice *dev) : _device(dev), _particleCount(0)			{}
		virtual ~ParticleRenderer();

		virtual const char*		getRendererName() const									{ return "null"; }

		virtual void			addParticle(Particle& p, const Camera& c)=0;
		virtual void			renderParticles()=0;
};
