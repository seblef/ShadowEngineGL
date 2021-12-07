
#pragma once

#include <string>

class GameEntityTrigger;
class GameCharacter;

namespace YAML
{
	class Node;
}

#define TRIGGER_REBIRTH_DELAY				5.0f

class Trigger
{
public:

	Trigger()			{}
	virtual ~Trigger()	{}

	virtual void		apply(GameCharacter* actor, GameEntityTrigger* entity) = 0;

	virtual void		load()						{}
	virtual void		unload()					{}
};

class TriggerFactory
{
public:

	TriggerFactory()			{}

	static Trigger*				createTrigger(const std::string& className, const YAML::Node& node);
};
