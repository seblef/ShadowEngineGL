
#include "Particles.h"
#include "IVisibilitySystem.h"
#include "Renderer.h"
#include "../particles/ParticleEngine.h"
#include "../particles/ParticleSystem.h"

RParticles::RParticles(ParticleSystem* sys, const Matrix4& world, bool alwaysVisible) :
	Renderable(sys->getWorldBBox(), world, alwaysVisible), _system(sys)
{
	sys->setWorldMatrix(world);
}

RParticles::~RParticles()
{
	delete _system;
}

void RParticles::setWorldMatrix(const Matrix4& world)
{
	_worldMatrix = world;
	_system->setWorldMatrix(world);
}

void RParticles::enqueue()
{
	ParticleEngine::getSingletonRef().enqueueSystem(_system);
}

void RParticles::update(float time)
{
	_system->update(time, *Renderer::getSingletonRef().getCurrentCamera());
	_worldBBox = _system->getWorldBBox();

	if (!_alwaysVisible)
		Renderer::getSingletonRef().getVisibilitySystem()->onRenderableMoved(this);
}
