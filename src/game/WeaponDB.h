
#pragma once

#include "Weapon.h"

class WeaponDB : public TDataBase<Weapon>
{
public:

	WeaponDB(const string& weaponFile);

	void				unloadWeapons();
};
