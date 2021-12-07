
#pragma once

#include "GameObject.h"
#include "../renderer/Light.h"


class GameLight : public GameObject
{
protected:

	Light*					_rLight;

public:

	GameLight(const LightCreate_t& c, Light::LightType type);
	~GameLight();

	void					onAddToScene();
	void					onRemFromScene();
	void					onUpdateMatrix();
};
