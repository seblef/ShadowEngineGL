
#include "Particles.h"
#include "IVisibilitySystem.h"
#include "Renderer.h"
#include "../particles/Engine.h"
#include "../particles/System.h"


RParticles::RParticles(
    Particles::System* sys,
    const Core::Matrix4& world,
    bool alwaysVisible
) :
	Renderable(sys->getWorldBBox(), world, alwaysVisible),
    _system(sys)
{
	sys->setWorldMatrix(world);
}

RParticles::~RParticles()
{
}

void RParticles::setWorldMatrix(const Core::Matrix4& world)
{
	_worldMatrix = world;
	_system->setWorldMatrix(world);
}

void RParticles::enqueue()
{
	Particles::Engine::getSingletonRef().enqueueSystem(_system.get());
}

void RParticles::update(float time)
{
	_system->update(time, *Renderer::getSingletonRef().getCurrentCamera());
	_worldBBox = _system->getWorldBBox();

	if (!_alwaysVisible)
		Renderer::getSingletonRef().getVisibilitySystem()->onRenderableMoved(this);
}
