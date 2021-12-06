
#include "EntityDynamic.h"
#include "GameEntityDynamic.h"
#include "../core/YAMLCore.h"


EntityDynamic::EntityDynamic(const YAML::Node& node) :
	EntityTemplate(false, node),
	_density(node["density"].as<float>(1.f))
{
}

void EntityDynamic::load()
{
	EntityTemplate::load();
}

void EntityDynamic::unload()
{
	EntityTemplate::unload();
}

GameEntity* EntityDynamic::createInstance(const Matrix4& world)
{
	return new GameEntityDynamic(*this, world);
}
