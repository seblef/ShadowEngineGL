
#pragma once

#include "Effect.h"
#include "../Core.h"

using namespace Core;

namespace YAML
{
	class Node;
}

class EffectFactory
{
public:

	EffectFactory()			{}

	static Effect*					createEffect(const string& effectName, const YAML::Node& node);
};
