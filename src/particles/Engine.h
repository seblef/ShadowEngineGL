#pragma once

#include "RendererPoint.h"
#include "RendererQuad.h"
#include "../core/TSingleton.h"
#include "../mediacommon/IVideoDevice.h"
#include <memory>
#include <vector>


class IShader;


namespace Particles
{

class Particle;
class System;

class Engine : public Core::TSingleton<Engine>
{
protected:
    IVideoDevice* _device;
    IShader* _shader;
    DepthStencilState _depthState;
    RenderState _renderState;
    SamplerState _samplerState;

    RendererPoint _pointRenderer;
    RendererQuad _quadRenderer;

    std::unique_ptr<Particle*[]> _particles;
    unsigned int _maxParticles;
    unsigned int _particlesOnScreen;

    std::vector<System*> _systems;

public:
    Engine(unsigned int maxParticles, IVideoDevice* device);
    ~Engine();

    unsigned int getMaxParticles() const { return _maxParticles; }
    unsigned int getParticlesOnScreen() const { return _particlesOnScreen; }

    void enqueueSystem(System *sys);
    void draw(const Core::Camera& camera);

    RendererPoint* getPointRenderer() { return &_pointRenderer; }
    RendererQuad* getQuadRenderer() { return &_quadRenderer; }
    ParticleRenderer* getRenderer(const std::string& renderer);

    IVideoDevice* getVideoDevice() const { return _device; }
};

}