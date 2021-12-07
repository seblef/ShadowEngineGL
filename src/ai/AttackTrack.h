
#pragma once

#include "AIMachineState.h"
#include "../core/Vector3.h"

using namespace Core;

class AttackTrack : public AIMachineState
{
protected:

	void					tryFire(AIAgent *a, const Vector3& playerPos,
		const Vector3& agentToPlayerDir, float distToPlayer);

public:

	AttackTrack(AIMachine* m) : AIMachineState(m)		{}

	bool					onEvent(AIAgent* a, const AIEvent& e);

	const char*				getStateName() const	{ return "track"; }
};
