
#include "Trigger.h"
#include "TriggerWeapon.h"

Trigger* TriggerFactory::createTrigger(const string& className, const YAML::Node& node)
{
	if (className == "weapon")
		return new TriggerWeapon(node);
	else
		return 0;
}
