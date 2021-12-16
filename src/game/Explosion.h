
#pragma once

#include "Action.h"
#include "../core/Vector3.h"
#include <memory>


class ExplosionTemplate;
class RParticles;


class Explosion : public Action
{
protected:

	const ExplosionTemplate* _template;
	Core::Vector3 _position;
	bool _exploded;

	std::unique_ptr<RParticles> _rParticles;

	void explode();

public:

	Explosion(
        const ExplosionTemplate& e,
        const Core::Vector3& pos
    );
	~Explosion();

	void update(float time);
};
