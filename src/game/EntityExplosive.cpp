
#include "EntityExplosive.h"
#include "EffectDB.h"
#include "ExplosionTemplate.h"
#include "GameEntityExplosive.h"
#include "GameSystem.h"
#include "../core/YAMLCore.h"


EntityExplosive::EntityExplosive(const YAML::Node& node) :
	EntityDynamic(node),
	_damageThreshold(node["damage_threshold"].as<float>(20.f)),
	_explosionName(node["explosion"].as<string>(""))
{
}

EntityExplosive::~EntityExplosive()
{
}

void EntityExplosive::load()
{
	EntityDynamic::load();

	_explosionTemplate = (ExplosionTemplate*)GameSystem::getSingletonRef().getEffectDB().getData(_explosionName);
	assert(_explosionTemplate);

	_explosionTemplate->load();
}

GameEntity* EntityExplosive::createInstance(const Matrix4& world)
{
	return new GameEntityExplosive(*this,world);
}

