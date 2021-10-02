
#pragma once

#include "AIMachineState.h"

class GazeTrack : public AIMachineState
{
public:

	GazeTrack(AIMachine* m) : AIMachineState(m)		{}
	~GazeTrack()									{}

	bool					onEvent(AIAgent* a, const AIEvent& e);

	const char*				getStateName() const	{ return "track"; }
};
