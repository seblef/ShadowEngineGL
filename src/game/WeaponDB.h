
#pragma once

#include "Weapon.h"
#include "../core/TDataBase.h"

class WeaponDB : public Core::TDataBase<Weapon>
{
public:

	WeaponDB(const string& weaponFile);

	void				unloadWeapons();
};
