
#pragma once

#include "AIMachineState.h"

class GazeIdle : public AIMachineState
{
public:

	GazeIdle(AIMachine* m) : AIMachineState(m)			{}

	bool						onEvent(AIAgent* a, const AIEvent& e);

	const char*					getStateName() const						{ return "idle"; }
};
