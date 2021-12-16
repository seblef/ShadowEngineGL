#include "PreviewParticles.h"
#include "PreviewResources.h"
#include "../mediacommon/IConstantBuffer.h"
#include "../mediacommon/IShader.h"
#include "../mediacommon/IVideoDevice.h"
#include "../particles/Engine.h"
#include "../particles/System.h"
#include "../renderer/SceneInfosBuffer.h"
#include "../loguru.hpp"

namespace Editor
{

PreviewParticles::PreviewParticles(
    Particles::SystemTemplate* particles,
    IVideoDevice* device,
    PreviewResources* resources,
    int width,
    int height
) :
    PreviewFrame(device, resources, width, height),
    _particles(0),
    _currentBBox(Core::BBox3::NullBox),
    _prevTime(-1.f)
{
    _sceneBuffer = std::unique_ptr<IConstantBuffer>(
        device->createConstantBuffer(sizeof(SceneInfosBuffer) / (4*sizeof(float)),0)
    );
    setSystem(particles);
}

PreviewParticles::~PreviewParticles()
{
    if(_particles)
        delete _particles;
}

void PreviewParticles::render(float time)
{
    float delta = 0.f;
    if(_prevTime > 0.f)
        delta = time - _prevTime;
    _prevTime = time;


    _device->clearRenderTargets(Core::Color::Black);
    _particles->update(delta, _camera.getCamera());
    LOG_S(INFO) << "Count: " << _particles->getParticlesCount();
    if(_particles->getParticlesCount() > 0)
    {
        const Core::BBox3& bbox(_particles->getWorldBBox());
        _currentBBox << bbox;
        _camera.center(_currentBBox);
    }
    Particles::Engine::getSingletonRef().enqueueSystem(_particles);

    _resources->getParticlesShader()->set();

    _camera.getCamera().buildMatrices();
    SceneInfosBuffer infoBuffer;
    infoBuffer.setCamera(_camera.getCamera());

    _sceneBuffer->set();
    _sceneBuffer->fill(&infoBuffer);

    Particles::Engine::getSingletonRef().draw(_camera.getCamera());
}

void PreviewParticles::setSystem(Particles::SystemTemplate* system)
{
    if(_particles)
        delete _particles;
    _particles = new Particles::System(*system);
}

}