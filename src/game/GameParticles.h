#pragma once

#include "GameObject.h"
#include <memory>

class RParticles;

namespace Particles
{
class SystemTemplate;
}

class GameParticles : public GameObject
{
protected:

	std::unique_ptr<RParticles> _system;

public:

	GameParticles(
        const Particles::SystemTemplate& t,
        const Core::Matrix4& world,
        bool alwaysVisible = false
    );
	~GameParticles();

	void onAddToScene();
	void onRemFromScene();
	void onUpdateMatrix(const Core::Matrix4& new_m)	{}
};
