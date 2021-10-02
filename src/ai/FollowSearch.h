
#pragma once

#include "AIMachineState.h"

class FollowSearch : public AIMachineState
{
public:

	FollowSearch(AIMachine* m) : AIMachineState(m)		{}

	bool					onEvent(AIAgent* a, const AIEvent& e);

	const char*				getStateName() const	{ return "search"; }

};
