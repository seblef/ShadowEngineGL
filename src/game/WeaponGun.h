
#pragma once

#include "Weapon.h"

class TraceTemplate;

class WeaponGun : public Weapon
{
protected:

	TraceTemplate*			_trace;

public:

	WeaponGun(const YAML::Node& node);
	~WeaponGun();

	TraceTemplate*			getTrace() const					{ return _trace; }

	WeaponInstance*			createInstance(GameCharacter* c);
};
