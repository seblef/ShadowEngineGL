
#pragma once

#include "Weapon.h"
#include "WeaponGunInstance.h"
#include "TraceTemplate.h"

class WeaponGun : public Weapon
{
protected:

	TraceTemplate*			_trace;

public:

	WeaponGun(const YAML::Node& node);
	~WeaponGun();

	TraceTemplate*			getTrace() const					{ return _trace; }

	WeaponInstance*			createInstance(GameCharacter* c)	{ return new WeaponGunInstance(this,c); }
};
