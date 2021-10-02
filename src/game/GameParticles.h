
#pragma once

#include "TemplateParticleSystem.h"
#include "GameObject.h"
#include "../StdRenderer.h"

class GameParticles : public GameObject
{
protected:

	Particles*				_system;

public:

	GameParticles(const TemplateParticleSystem& t, const Matrix4& world, bool alwaysVisible = false);
	~GameParticles();

	void						onAddToScene();
	void						onRemFromScene();
	void						onUpdateMatrix(const Matrix4& new_m)	{}
};
