
#pragma once

#include "Action.h"


class GameObject;

class AddRemObject : public Action
{
protected:

	GameObject*			_object;
	bool				_remove;
	bool				_delete;

public:

	AddRemObject(GameObject* o, bool rem = false, bool del = false);
	~AddRemObject();

	void				update(float time);
};
