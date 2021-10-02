
#pragma once

#include "Effect.h"
#include "../Core.h"

using namespace Core;

class EffectDB : public TDataBase<Effect>
{
public:

	EffectDB(const string& effectFile);

	void			unloadEffects() const;
};
