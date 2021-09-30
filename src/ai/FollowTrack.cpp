
#include "FollowTrack.h"
#include "AIAgent.h"
#include "Ids.h"
#include "AISystem.h"
#include "../loguru.hpp"
#include "../NavigationLib.h"


#define FOLLOW_MIN_DIST				1.0f
#define FOLLOW_MAX_DIST				4.0f
#define FOLLOW_DESIRED_DIST			2.5f

bool FollowTrack::onEvent(AIAgent* a, const AIEvent& e)
{
	Vector3 dir;
	Vector3 playerPos;
	float dist;
	const Vector3& pos(a->getCallback()->getAgentPosition());

	switch (e.getName())
	{
	case AIEVT_ENTER:
#ifdef _DEBUG
		LOG_S(1) << "Agent " << a->getID() << "(" << getStateName() << ") receiving ENTER event.";
#endif
	case AIEVT_UPDATE:
		playerPos = AISystem::getSingletonRef().getPlayerPos();
		dir = playerPos - pos;
		dist = dir.getLength();
		dir.normalize();

		int destx, desty;

		if (dist <= FOLLOW_MIN_DIST || dist >= FOLLOW_MAX_DIST)
		{
			destx = (int)(playerPos.x - dir.x * FOLLOW_DESIRED_DIST);
			desty = (int)(playerPos.z - dir.z * FOLLOW_DESIRED_DIST);

			if (Navigation::getSingletonRef().getMap().findBestPlace(destx, desty, dir.x, dir.z, destx, desty))
				a->getCallback()->runTo(Vector3((float)destx, 0, (float)desty), dir);
		}
		else
			a->getCallback()->orientTo(dir);

		a->setLastKnownPlayerPosition(AISystem::getSingletonRef().getPlayerPos());

		return true;
		break;

	case AIEVT_LOSTPLAYER:
#ifdef _DEBUG
		LOG_S(1) << "Agent " << a->getID() << "(" << getStateName() << ") recieving LOSTPLAYER event.";
#endif
		_machine->setState(a, FOLLOW_SEARCH_STATE_ID);
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
