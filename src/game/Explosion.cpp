
#include "Explosion.h"
#include "ExplosionTemplate.h"
#include "Debris.h"
#include "ActionServer.h"
#include "FieldHit.h"
#include "Flash.h"
#include "TemplateMesh.h"
#include "../mediacommon/ISoundVirtualSource.h"
#include "../particles/System.h"
#include "../particles/Emitters.h"
#include "../renderer/Renderer.h"
#include "../renderer/Particles.h"
#include "../sound/SoundSystem.h"


const float ExplosionFlashAscend = 0.1f;
const float ExplosionFlashStable = 0.1f;
const float ExplosionFlashDescend = 0.1f;

Explosion::Explosion(
    const ExplosionTemplate& e,
    const Core::Vector3& pos
) :
    Action("Explosion"),
    _template(&e),
	_position(pos),
    _exploded(false)
{
}

Explosion::~Explosion()
{
	if (_rParticles)
	{
		_rParticles->sleep();
		Renderer::getSingletonRef().remRenderable(_rParticles.get());
	}
}

void Explosion::update(float time)
{
	if (!_exploded)
	{
		explode();
		_exploded = true;
	}

	_dead = _rParticles->getParticleSystem()->getParticlesCount() <= 0;
}

void Explosion::explode()
{
	Matrix4 world;
	world.createTranslate(_position.x, _position.y, _position.z);

	if (_template->getParticles()->getSubCount() > 0)
	{
        Particles::System* sys = new Particles::System(*_template->getParticles());
		sys->setWorldMatrix(world);
		_rParticles = std::unique_ptr<RParticles>(new RParticles(sys, world, true));

		if (_template->getExplosionEmitter() != -1)
		{
            Particles::SubSystem *explosion = sys->getSubSystem(_template->getExplosionEmitter());
            Particles::ExplosionEmitter* emitter = (Particles::ExplosionEmitter*)explosion->getEmitter();

            emitter->setSizes(_template->getExplosionSize(), _template->getExplosionSize());
            explosion->emitAll();
            sys->countParticles();
		}

		_rParticles->wakeUp();
		Renderer::getSingletonRef().addRenderable(_rParticles.get());
	}

	if (_template->getDebrisMesh() && _template->getDebrisCount() > 0)
	{
		ActionServer::getSingletonRef().addAction(
            new Debris(
                _template->getDebrisMesh()->getMesh(),
			    _template->getDebrisCount(),
                _position,
                Core::Vector3::YAxisVector,
			    _template->getDebrisMaxAngle(),
                _template->getDebrisMinVelocity(),
			    _template->getDebrisMaxVelocity(),
			    _template->getDebrisMaxAngleSpeed(),
                _template->getDebrisSize()
			)
        );
	}

	if (_template->getExplosionSound())
	{
		ISoundVirtualSource *src = SoundSystem::getSingletonRef().play(
			_template->getExplosionSound(),
			1, 10.f,
			_position,
			Vector3::NullVector,
			Vector3::ZAxisVector,
			10.f, 50.f,
			1.f, 0.f, 360.f
		)->getSource();
		src->setPosition(_position);
		src->setMaxDistance(50.0f);
	}

	if (_template->getHitEnvironment())
	{
		FieldHit::hitField(_position,
			_template->getInnerRadius(),
			_template->getOuterRadius(),
			_template->getInnerDamage());
	}

	ActionServer::getSingletonRef().addAction(new Flash(_template->getExplosionColor(),
		ExplosionFlashAscend, ExplosionFlashStable, ExplosionFlashDescend,
		_template->getOuterRadius() * 1.5f,
		_position));
}
