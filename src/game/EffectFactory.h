
#pragma once

#include <string>

class Effect;

namespace YAML
{
	class Node;
}

class EffectFactory
{
public:

	EffectFactory()			{}

	static Effect*					createEffect(const std::string& effectName, const YAML::Node& node);
};
