
#pragma once

#include "../core/TSingleton.h"
#include <set>

class GameEntity;

class EntityUpdater : public Core::TSingleton<EntityUpdater>
{
protected:

	typedef std::set<GameEntity*>	EntitySet;
	EntitySet					_entities;

public:

	EntityUpdater()				{}

	void						addEntity(GameEntity* e);
	void						remEntity(GameEntity* e);

	void						updateEntities(float time) const;

	void						clear();
};
