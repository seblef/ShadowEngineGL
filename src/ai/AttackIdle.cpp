
#include "AttackIdle.h"
#include "AIAgent.h"
#include "Ids.h"
#include "../loguru.hpp"


bool AttackIdle::onEvent(AIAgent* a, const AIEvent& e)
{
	switch (e.getName())
	{
	case AIEVT_ENTER:
#ifdef _DEBUG
		LOG_S(1) << "Agent " << a->getID() << " (" << getStateName() << ") recieving ENTER event.";
#endif
		a->getCallback()->idle();
		return true;
		break;

	case AIEVT_SEEPLAYER:
#ifdef _DEBUG
		LOG_S(1) << "Agent " << a->getID() << " (" << getStateName() << ") recieving SEEPLAYER event.";
#endif
		_machine->setState(a, ATTACK_TRACK_STATE_ID);
		return true;
		break;

	case AIEVT_HIT:
#ifdef _DEBUG
		int damage = e.getIntParam();
		LOG_S(1) << "Agent" << a->getID() << "hitted by agent " << e.getSenderID() << "(damage: " << *((float*)&damage) << ")";
#endif
		return true;
	}

	return false;
}
