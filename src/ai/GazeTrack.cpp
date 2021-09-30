
#include "GazeTrack.h"
#include "AIAgent.h"
#include "Ids.h"
#include "AISystem.h"
#include "../loguru.hpp"


bool GazeTrack::onEvent(AIAgent* a, const AIEvent& e)
{
	Vector3 dir;

	switch (e.getName())
	{
	case AIEVT_ENTER:
#ifdef _DEBUG
		LOG_S(1) << "Agent " << a->getID() << " (" << getStateName() << ") recieving ENTER event.";
#endif
	case AIEVT_UPDATE:
		dir = AISystem::getSingletonRef().getPlayerPos() - a->getCallback()->getAgentPosition();
		dir.normalize();
		a->getCallback()->orientTo(dir);
		return true;

	case AIEVT_LOSTPLAYER:
#ifdef _DEBUG
		LOG_S(1) << "Agent " << a->getID() << " (" << getStateName() << ") recieving SEEPLAYER event.";
#endif
		_machine->setState(a, GAZE_IDLE_STATE_ID);
		return true;
	}

	return false;
}
