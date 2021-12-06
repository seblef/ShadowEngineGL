
#include "EffectFactory.h"
#include "ExplosionTemplate.h"

Effect* EffectFactory::createEffect(const string& effectName, const YAML::Node& node)
{
	if (effectName == "explosion")
		return new ExplosionTemplate(node);
	else
		return 0;
}
