
#pragma once

#include "Effect.h"
#include "../Core.h"

using namespace Core;

class EffectFactory
{
public:

	EffectFactory()			{}

	static Effect*					createEffect(const string& effectName, ScriptFile& sf);
};
