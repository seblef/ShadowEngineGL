
#pragma once

#include "Weapon.h"

class WeaponFactory
{
public:

	WeaponFactory()				{}

	static Weapon*				loadWeapon(const string& className, ScriptFile& sf);
};
