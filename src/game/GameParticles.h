
#pragma once

#include "GameObject.h"

class RParticles;
class TemplateParticleSystem;

class GameParticles : public GameObject
{
protected:

	RParticles*				_system;

public:

	GameParticles(const TemplateParticleSystem& t, const Matrix4& world, bool alwaysVisible = false);
	~GameParticles();

	void						onAddToScene();
	void						onRemFromScene();
	void						onUpdateMatrix(const Matrix4& new_m)	{}
};
