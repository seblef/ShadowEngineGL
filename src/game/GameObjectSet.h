
#pragma once

#include "GameObject.h"

class GameObjectSet
{
protected:

	typedef TVector<GameObject>				ObjectVector;
	typedef TSet<GameObject>				ObjectSet;

	ObjectVector		_staticObjects;
	ObjectSet			_tempObjects;

public:

	GameObjectSet();
	virtual ~GameObjectSet();

	void				addObject(GameObject* o, bool temp);
	void				remObject(GameObject *o)		{ o->onRemFromScene(); _tempObjects.getSet().erase(o); }

};
