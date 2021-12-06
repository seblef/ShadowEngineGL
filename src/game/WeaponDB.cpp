
#include "WeaponDB.h"
#include "WeaponFactory.h"
#include "../core/YAMLCore.h"
#include "../loguru.hpp"


WeaponDB::WeaponDB(const string& weaponFile)
{
	YAML::Node root;
	try
	{
		root = YAML::LoadFile(weaponFile);
	}
	catch(const std::exception& e)
	{
		LOG_S(ERROR) << e.what();
		return;
	}

	for(YAML::const_iterator w=root.begin(); w!=root.end(); ++w)
	{
		const string& name(w->first.as<string>());
		const string& className(w->second["class"].as<string>());

		Weapon *weapon = WeaponFactory::loadWeapon(className, w->second);
		registerData(name, weapon);
	}
}

void WeaponDB::unloadWeapons()
{
	for(auto const& w : _data)
		w.second->unload();
}
