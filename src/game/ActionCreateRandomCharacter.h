
#pragma once

#include "Action.h"
#include "../core/Vector2.h"

using namespace Core;

class ActionCreateRandomCharacter : public Action
{
protected:

	void				findPosAndOrientation(Vector2& pos, float& orientation) const;

public:

	ActionCreateRandomCharacter() : Action("CreateRandomCharacter")			{}

	void				update(float time);
};
