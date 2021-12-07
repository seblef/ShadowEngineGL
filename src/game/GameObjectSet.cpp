
#include "GameObjectSet.h"

GameObjectSet::GameObjectSet()
{

}

GameObjectSet::~GameObjectSet()
{
	_staticObjects.clear();
	_tempObjects.clear();
}

void GameObjectSet::addObject(GameObject* o, bool temp)
{
	if (temp)		_tempObjects.getSet().insert(o);
	else			_staticObjects.getVector().push_back(o);

	o->onAddToScene();
}

void GameObjectSet::remObject(GameObject *o)
{
    o->onRemFromScene();
    _tempObjects.getSet().erase(o);
}
