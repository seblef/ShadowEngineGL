
#include "Trace.h"
#include "TraceTemplate.h"
#include "GameCamera.h"
#include "../particles/System.h"
#include "../particles/Emitters.h"
#include "../renderer/Renderer.h"
#include "../renderer/Particles.h"


Trace::Trace(
    const TraceTemplate& t,
    const Core::Vector3& p1,
    const Core::Vector3& p2
) : Action("Trace")
{
	Particles::System *ps = new Particles::System(*t.getParticleSystem());
	float count = (p1 - p2).getLength() * t.getDensity();

    ps->setWorldMatrix(Core::Matrix4::Identity);
	for (int i = 0; i < ps->getSubCount(); ++i)
	{
        Particles::SubSystem *sub = ps->getSubSystem(i);
        Particles::LineEmitter *emitter = (Particles::LineEmitter*)sub->getEmitter();
        emitter->setPositions(p1, p2);
        sub->emitAll();
	}

    ps->countParticles();
	_particleSystem = std::unique_ptr<RParticles>(new RParticles(ps, Core::Matrix4::Identity, true));
	_particleSystem->wakeUp();
	Renderer::getSingletonRef().addRenderable(_particleSystem.get());
}

Trace::~Trace()
{
	_particleSystem->sleep();
	Renderer::getSingletonRef().remRenderable(_particleSystem.get());
}

void Trace::update(float time)
{
	_dead = _particleSystem->getParticleSystem()->getParticlesCount() == 0;
}
