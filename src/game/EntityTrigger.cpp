
#include "EntityTrigger.h"
#include "GameEntityTrigger.h"
#include "../core/YAMLCore.h"


EntityTrigger::EntityTrigger(const YAML::Node& node) :
	EntityTemplate(true, node),
	_trigger(0),
	_animated(node["animated"].as<bool>(false))
{
    YAML::Node trigger = node["trigger"];
	if(trigger)
		_trigger = TriggerFactory::createTrigger(
			trigger["class"].as<string>(),
			trigger
		);
}

EntityTrigger::~EntityTrigger()
{
	if (_trigger)			delete _trigger;
}

void EntityTrigger::load()
{
	EntityTemplate::load();
	if (_trigger)		_trigger->load();
}

void EntityTrigger::unload()
{
	EntityTemplate::unload();
	if (_trigger)		_trigger->unload();
}

GameEntity* EntityTrigger::createInstance(const Matrix4& world)
{
	return new GameEntityTrigger(*this, world);
}
