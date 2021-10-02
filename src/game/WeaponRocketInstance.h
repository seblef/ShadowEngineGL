
#pragma once

#include "WeaponInstance.h"

class WeaponRocketInstance : public WeaponInstance
{
public:

	WeaponRocketInstance(Weapon* w, GameCharacter* owner);

	void				fire();
};
