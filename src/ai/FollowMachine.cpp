
#include "FollowMachine.h"
#include "FollowTrack.h"
#include "FollowIdle.h"
#include "FollowSearch.h"

FollowMachine::FollowMachine() : AIMachine(FOLLOW_MACHINE_ID)
{
	_states[FOLLOW_IDLE_STATE_ID] = new FollowIdle(this);
	_states[FOLLOW_TRACK_STATE_ID] = new FollowTrack(this);
	_states[FOLLOW_SEARCH_STATE_ID] = new FollowSearch(this);
}

unsigned int FollowMachine::getStateID(const string& name)
{
    if (name == "idle")
        return FOLLOW_IDLE_STATE_ID;
    else if (name == "track")
        return FOLLOW_TRACK_STATE_ID;
    else if (name == "search")
        return FOLLOW_SEARCH_STATE_ID;
    else
        return 0xffffffff;
}
