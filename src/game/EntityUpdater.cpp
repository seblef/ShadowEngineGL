
#include "EntityUpdater.h"
#include "GameEntity.h"

void EntityUpdater::updateEntities(float time) const
{
    for(auto const& e : _entities)
		e->update(time);
}

void EntityUpdater::addEntity(GameEntity* e)
{
    _entities.insert(e);
}

void EntityUpdater::remEntity(GameEntity* e)
{
    _entities.erase(e);
}

void EntityUpdater::clear()
{
    _entities.clear();
}
