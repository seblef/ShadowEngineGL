#pragma once

#include "Renderable.h"
#include "IUpdatable.h"

class ParticleSystem;

class RParticles : public Renderable, public IUpdatable
{
protected:

	ParticleSystem*				_system;

public:

	RParticles(ParticleSystem *system, const Matrix4& world, bool alwaysVisible=false);
	~RParticles();

	void						setWorldMatrix(const Matrix4& world);
	void						enqueue();
	void						update(float time);

	ParticleSystem*				getParticleSystem() const					{ return _system; }
};
