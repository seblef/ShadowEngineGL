
#include "EffectDB.h"
#include "EffectFactory.h"
#include "../core/YAMLCore.h"
#include "../loguru.hpp"


EffectDB::EffectDB(const string& effectsFile)
{
	YAML::Node root;
	try
	{
		root = YAML::LoadFile(effectsFile);
	}
	catch(const std::exception& e)
	{
		LOG_S(ERROR) << e.what();
		return;
	}

	for(YAML::const_iterator e=root.begin(); e!=root.end(); ++e)
		registerData(
			e->first.as<string>(),
			EffectFactory::createEffect(e->second["class"].as<string>(), e->second)
		);
}

void EffectDB::unloadEffects() const
{
	for(auto const& e : _data)
		e.second->unload();
}
