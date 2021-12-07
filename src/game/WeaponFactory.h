
#pragma once

#include <string>

class Weapon;

namespace YAML
{
    class Node;
}

class WeaponFactory
{
public:

	WeaponFactory()				{}

	static Weapon*				loadWeapon(const std::string& className, const YAML::Node& node);
};
