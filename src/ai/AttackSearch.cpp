
#include "AttackSearch.h"
#include "AIAgent.h"
#include "Ids.h"
#include "AISystem.h"
#include "../loguru.hpp"


bool AttackSearch::onEvent(AIAgent* a, const AIEvent& e)
{
	Vector3 dir;
	const Vector3& pos(a->getCallback()->getAgentPosition());

	switch (e.getName())
	{
	case AIEVT_ENTER:
#ifdef _DEBUG
		LOG_S(1) << "Agent " << a->getID() << " (" << getStateName() << ") recieving ENTER event.";
#endif
		dir = a->getLastKnownPlayerPosition() - pos;
		dir.normalize();
		a->getCallback()->runTo(a->getLastKnownPlayerPosition(), dir);
		return true;
		break;

	case AIEVT_SEEPLAYER:
#ifdef _DEBUG
		LOG_S(1) << "Agent " << a->getID() << " (" << getStateName() << ") recieving SEEPLAYER event.";
#endif
		_machine->setState(a, ATTACK_TRACK_STATE_ID);
		return true;
		break;

	case AIEVT_ENDMOVE:
#ifdef _DEBUG
		LOG_S(1) << "Agent " << a->getID() << " (" << getStateName() << ") recieving ENDMOVE event. Going to home position.";
#endif
		a->getCallback()->goHome();
		_machine->setState(a, ATTACK_IDLE_STATE_ID);
		return true;

	case AIEVT_HIT:
#ifdef _DEBUG
		int damage = e.getIntParam();
		LOG_S(1) << "Agent" << a->getID() << "hitted by agent " << e.getSenderID() << "(damage: " << *((float*)&damage) << ")";
#endif
		return true;
	}

	return false;
}
