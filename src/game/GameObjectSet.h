
#pragma once

#include "GameObject.h"
#include "../core/TSet.h"
#include "../core/TVector.h"

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
	void				remObject(GameObject *o);

};
