
#include "WeaponGun.h"
#include "TraceTemplate.h"
#include "WeaponGunInstance.h"
#include "../core/YAMLCore.h"


WeaponGun::WeaponGun(const YAML::Node& node) : Weapon(node), _trace(0)
{
    YAML::Node trace = node["trace"];
	if(trace)
		_trace = new TraceTemplate(trace);
}

WeaponGun::~WeaponGun()
{
	if (_trace)
		delete _trace;
}

WeaponInstance* WeaponGun::createInstance(GameCharacter* c)
{
    return new WeaponGunInstance(this,c);
}
