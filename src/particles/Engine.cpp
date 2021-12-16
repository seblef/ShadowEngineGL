#include "Engine.h"
#include "Particle.h"
#include "System.h"
#include "../mediacommon/IShader.h"
#include "../loguru.hpp"
#include <algorithm>


namespace Particles
{

Engine::Engine(unsigned int maxParticles, IVideoDevice* device) :
    _device(device),
    _pointRenderer(device),
    _quadRenderer(device),
    _maxParticles(maxParticles),
    _particlesOnScreen(0)
{
    _depthState = device->createDepthStencilState(true, false, COMP_LESSEQUAL);
    _renderState = device->createRenderState(CULL_NONE);
    _samplerState = device->createSamplerState(FILTER_TRILINEAR);
    _shader = device->createShader("Base/Particles");

    _particles = std::unique_ptr<Particle*[]>(new Particle*[maxParticles]);
}

Engine::~Engine()
{
    _device->destroyDepthStencilState(_depthState);
    _device->destroyRenderState(_renderState);
    _device->destroySamplerState(_samplerState);
    _shader->remRef();
}

void Engine::enqueueSystem(System* sys)
{
    _systems.push_back(sys);
}

void Engine::draw(const Core::Camera& camera)
{
	_particlesOnScreen = 0;
    for(auto const& sys : _systems)
		sys->collectParticles(
            _particles.get(),
            _particlesOnScreen,
            _maxParticles
        );

	_systems.clear();

	if(_particlesOnScreen == 0)
		return;

	std::sort(
        _particles.get(),
        _particles.get() + _particlesOnScreen,
        [](const Particle* p1, const Particle* p2){ return p1->_distFromCam > p2->_distFromCam; }
    );

    _device->setDepthStencilState(_depthState);
    _device->setRenderState(_renderState);
    _device->setSamplerState(0, _samplerState);
	_shader->set();

	unsigned int i = 0;
	ParticleRenderer *r;
	while(i < _particlesOnScreen)
	{
		r = _particles[i]->_renderer;
		while(i < _particlesOnScreen && _particles[i]->_renderer==r)
		{
			r->addParticle(*_particles[i], camera);
			++i;
		}

		r->renderParticles();
	}
}

ParticleRenderer* Engine::getRenderer(const std::string& renderer)
{
    if(renderer == "quad")
        return &_quadRenderer;
    else
        return &_pointRenderer;
}

}