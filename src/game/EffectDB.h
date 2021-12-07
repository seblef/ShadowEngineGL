
#pragma once

#include "Effect.h"
#include "../core/TDataBase.h"

using namespace Core;

class EffectDB : public Core::TDataBase<Effect>
{
public:

	EffectDB(const string& effectFile);

	void			unloadEffects() const;
};
