
#include "TriggerWeapon.h"
#include "GameSystem.h"
#include "GameEntityTrigger.h"
#include "Map.h"
#include "ActionServer.h"
#include "AddRemObject.h"
#include "../core/YAMLCore.h"


TriggerWeapon::TriggerWeapon(const YAML::Node& node) :
	_noRebirth(node["no_rebirth"].as<bool>(false)),
	_weapon(node["weapon"].as<string>())
{
}

void TriggerWeapon::apply(GameCharacter* actor, GameEntityTrigger* entity)
{
	Weapon *w = GameSystem::getSingletonRef().getWeaponDB().getData(_weapon);
	assert(w);
	actor->setWeapon(w);

	GameSystem::getSingletonRef().getMap().remObject(entity);
	if (_noRebirth)
		delete entity;
	else
		ActionServer::getSingletonRef().addDelayedAction(new AddRemObject(entity),TRIGGER_REBIRTH_DELAY);
}
