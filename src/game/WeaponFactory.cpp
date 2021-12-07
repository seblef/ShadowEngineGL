
#include "WeaponFactory.h"
#include "WeaponGun.h"
#include "WeaponRocket.h"


Weapon* WeaponFactory::loadWeapon(const std::string& className, const YAML::Node& node)
{
	Weapon* w=0;
	if (className == "gun")
		w = new WeaponGun(node);
	else if (className == "rocket")
		w = new WeaponRocket(node);

	return w;
}
