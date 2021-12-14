
#include "RendererPoint.h"
#include "Particle.h"
#include "../mediacommon/ITexture.h"
#include "../mediacommon/IVertexBuffer.h"


#define PRPOINT_PARTICLE_COUNT					1024

namespace Particles
{

RendererPoint::RendererPoint(IVideoDevice* d) : ParticleRenderer(d), _lastBlend(0)
{
	_vBuffer = std::unique_ptr<IVertexBuffer>(d->createVertexBuffer(PRPOINT_PARTICLE_COUNT,VX_BASE,BU_DYNAMIC));
	_vertices = std::unique_ptr<Core::VxBase[]>(new Core::VxBase[PRPOINT_PARTICLE_COUNT]);
	_whiteTexture=d->createTexture("Textures/Default/Diffuse.bmp");
}

RendererPoint::~RendererPoint()
{
	_whiteTexture->remRef();
}

void RendererPoint::addParticle(Particle& p, const Camera& c)
{
	Core::VxBase& v(_vertices[_particleCount]);

	v.pos = p._pos;
	v.color = p._color;

	_lastBlend = p._material->getBlendState();

	++_particleCount;
	if(_particleCount >= PRPOINT_PARTICLE_COUNT)
		renderParticles();
}

void RendererPoint::renderParticles()
{
	if(_particleCount > 0)
	{
		_vBuffer->set();
		_vBuffer->fill(_vertices.get(),_particleCount);
		_device->setInputLayer(VX_BASE);
		_device->setPrimitiveType(PT_POINTLIST);
		_device->setBlendState(_lastBlend);
		_whiteTexture->set(0);
		
		_device->render(_particleCount);

		_particleCount=0;
	}
}

}