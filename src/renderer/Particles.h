#pragma once

#include "Renderable.h"
#include "IUpdatable.h"
#include <memory>


namespace Particles
{
class System;
}

class RParticles : public Renderable, public IUpdatable
{
protected:

	std::unique_ptr<Particles::System> _system;

public:

	RParticles(
        Particles::System *system,
        const Core::Matrix4& world,
        bool alwaysVisible=false
    );
	~RParticles();

	void setWorldMatrix(const Core::Matrix4& world);
	void enqueue();
	void update(float time);

	Particles::System* getParticleSystem() const { return _system.get(); }
};
