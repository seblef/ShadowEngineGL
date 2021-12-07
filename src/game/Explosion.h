
#pragma once

#include "Action.h"
#include "../core/Vector3.h"


class ExplosionTemplate;
class ParticlePointEmitter;
class Particles;

using namespace Core;

class Explosion : public Action
{
protected:

	const ExplosionTemplate*	_template;
	Vector3						_position;
	bool						_exploded;

	Particles*					_rParticles;

	ParticlePointEmitter*		_explosionEmitter;
	ParticlePointEmitter*		_smokeEmitter;

	float						_smokeTime;

	void						explode();

public:

	Explosion(const ExplosionTemplate& e, const Vector3& pos);
	~Explosion();

	void						update(float time);
};
