
#pragma once

#include "GameEntity.h"
#include "EntityTrigger.h"

class GameEntityTrigger : public GameEntity
{
protected:

	float				_rotation;

public:

	GameEntityTrigger(EntityTrigger& t, const Matrix4& world);

	void				update(float time);
	void				onTrigger(GameCharacter* actor);
};
