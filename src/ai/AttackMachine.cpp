
#include "AttackMachine.h"
#include "AttackTrack.h"
#include "AttackIdle.h"
#include "AttackSearch.h"

AttackMachine::AttackMachine() : AIMachine(ATTACK_MACHINE_ID)
{
	_states[ATTACK_IDLE_STATE_ID] = new AttackIdle(this);
	_states[ATTACK_TRACK_STATE_ID] = new AttackTrack(this);
	_states[ATTACK_SEARCH_STATE_ID] = new AttackSearch(this);
}

unsigned int AttackMachine::getStateID(const string& name)
{
    if (name == "idle")
        return ATTACK_IDLE_STATE_ID;
    else if (name == "track")
        return ATTACK_TRACK_STATE_ID;
    else if (name == "search")
        return ATTACK_SEARCH_STATE_ID;
    else
        return 0xffffffff;
}
