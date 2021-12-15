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
    _currentBBox(Core::BBox3::NullBox),
    _prevTime(-1.f)
{
    _particles = std::unique_ptr<Particles::System>(
        new Particles::System(*particles)
    );
    _sceneBuffer = std::unique_ptr<IConstantBuffer>(
        device->createConstantBuffer(sizeof(SceneInfosBuffer) / (4*sizeof(float)),0)
    );
}

PreviewParticles::~PreviewParticles()
{
}

void PreviewParticles::render(float time)
{
    float delta = 0.f;
    if(_prevTime > 0.f)
        delta = time - _prevTime;
    _prevTime = time;


    _device->clearRenderTargets(Core::Color::Black);
    _particles->update(delta, _camera.getCamera());
    if(_particles->getParticlesCount() > 0)
    {
        const Core::BBox3& bbox(_particles->getWorldBBox());
        _currentBBox << bbox;
        _camera.center(_currentBBox);
    }
    Particles::Engine::getSingletonRef().enqueueSystem(_particles.get());

    _resources->getParticlesShader()->set();

    _camera.getCamera().buildMatrices();
    SceneInfosBuffer infoBuffer;
    infoBuffer.setCamera(_camera.getCamera());

    _sceneBuffer->set();
    _sceneBuffer->fill(&infoBuffer);

    Particles::Engine::getSingletonRef().draw(_camera.getCamera());
}

}