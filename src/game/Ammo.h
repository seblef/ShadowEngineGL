
#pragma once

#include "Action.h"

class IPhysicObject;

class Ammo : public Action
{
public:

	Ammo() : Action("Ammo")			{}
	virtual ~Ammo()	{}

	virtual void	onContact(IPhysicObject* other)			{}
};
