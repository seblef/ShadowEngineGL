#pragma once

#include "../ParticleLib.h"
#include "Renderable.h"
#include "IUpdatable.h"

class Particles : public Renderable, public IUpdatable
{
protected:

	ParticleSystem*				_system;

public:

	Particles(ParticleSystem *system, const Matrix4& world, bool alwaysVisible=false);
	~Particles();

	void						setWorldMatrix(const Matrix4& world);
	void						enqueue();
	void						update(float time);

	ParticleSystem*				getParticleSystem() const					{ return _system; }
};
