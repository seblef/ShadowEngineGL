
#include "GameParticles.h"
#include "../particles/System.h"
#include "../renderer/Particles.h"
#include "../renderer/Renderer.h"


GameParticles::GameParticles(
    const Particles::SystemTemplate& t,
    const Core::Matrix4& world,
    bool alwaysVisible
) : 
    GameObject(PARTICLES, world, Core::BBox3::NullBox, 0)
{
    Particles::System *sys = new Particles::System(t);
	sys->setWorldMatrix(world);
	_system = std::unique_ptr<RParticles>(new RParticles(sys, world, alwaysVisible));
}

GameParticles::~GameParticles()
{
}

void GameParticles::onAddToScene()
{
	Renderer::getSingletonRef().addRenderable(_system.get());
	_system->wakeUp();
}

void GameParticles::onRemFromScene()
{
	_system->sleep();
	Renderer::getSingletonRef().remRenderable(_system.get());
}
