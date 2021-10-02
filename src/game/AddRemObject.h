
#pragma once

#include "Action.h"
#include "GameObject.h"

class AddRemObject : public Action
{
protected:

	GameObject*			_object;
	bool				_remove;
	bool				_delete;

public:

	AddRemObject(GameObject* o, bool rem = false, bool del = false) : Action("AddRemObject"), _object(o), _remove(rem), _delete(del)		{}
	~AddRemObject()
	{
		if (_object)
			delete _object;
	}

	void				update(float time);
};
