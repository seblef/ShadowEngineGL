
#include "AddRemObject.h"
#include "GameSystem.h"
#include "Map.h"

AddRemObject::AddRemObject(GameObject* o, bool rem, bool del) :
    Action("AddRemObject"),
    _object(o),
    _remove(rem),
    _delete(del)
{
}

AddRemObject::~AddRemObject()
{
    if (_object)
        delete _object;
}

void AddRemObject::update(float time)
{
	if (_object)
	{
		if (_remove)
		{
			GameSystem::getSingletonRef().getMap().remObject(_object);
			if (_delete)
				delete _object;
		}
		else
			GameSystem::getSingletonRef().getMap().addObject(_object,true);

		_object = 0;
	}

	_dead = true;
}
