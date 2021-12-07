
#pragma once

#include "Trigger.h"

class TriggerWeapon : public Trigger
{
protected:

	std::string			_weapon;
	bool				_noRebirth;

public:

	TriggerWeapon(const YAML::Node& node);

	void				apply(GameCharacter* actor, GameEntityTrigger* entity);
};
