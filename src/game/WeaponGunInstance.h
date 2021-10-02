
#pragma once

#include "WeaponInstance.h"

class WeaponGunInstance : public WeaponInstance
{
protected:

	void					damageTarget(Hit& hit, const Vector3& origin) const;

public:

	WeaponGunInstance(Weapon* w, GameCharacter* c) : WeaponInstance(w,c)			{}

	void					fire();
};
