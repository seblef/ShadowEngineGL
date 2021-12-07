#pragma once

#include "Renderable.h"
#include "IUpdatable.h"

class ParticleSystem;

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
