
#include "EntityFactory.h"
#include "EntityExplosive.h"
#include "EntityTrigger.h"

EntityTemplate* EntityFactory::loadEntity(const string& className, const YAML::Node& node)
{
	EntityTemplate* et = 0;
	if (className == "dynamic")
		et = new EntityDynamic(node);
	else if (className == "explosive")
		et = new EntityExplosive(node);
	else if (className == "trigger")
		et = new EntityTrigger(node);

	return et;
}
