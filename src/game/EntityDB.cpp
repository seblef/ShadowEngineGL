
#include "EntityDB.h"
#include "EntityFactory.h"
#include "../core/YAMLCore.h"
#include "../loguru.hpp"


EntityDB::EntityDB(const string& entityFile)
{
	YAML::Node root;
	try
	{
		root = YAML::LoadFile(entityFile);
	}
	catch(const std::exception& e)
	{
		LOG_S(ERROR) << e.what();
		return;
	}

	for(YAML::const_iterator e=root.begin(); e!=root.end(); ++e)
		registerData(
			e->first.as<string>(),
			EntityFactory::loadEntity(e->second["class"].as<string>(), e->second)
		);
}

void EntityDB::unloadEntities()
{
	for(auto const& e : _data)
		e.second->unload();
}
