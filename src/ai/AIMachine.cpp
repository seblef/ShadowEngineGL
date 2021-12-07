
#include "AIMachine.h"
#include "AIAgent.h"
#include "../loguru.hpp"
#include <assert.h>


AIMachine::~AIMachine()
{
    for(auto const& s : _states)
		delete s.second;
}

AIMachineState* AIMachine::getState(unsigned int stateID) const
{
	StateMap::const_iterator s(_states.find(stateID));
	return s == _states.end() ? 0 : s->second;
}

void AIMachine::setState(AIAgent* agent, unsigned int stateID) const
{
	setState(agent, getState(stateID));
}

void AIMachine::setState(AIAgent* agent, AIMachineState* state) const
{
	assert(state && state->getMachine()==this);

#ifdef _DEBUG
	LOG_S(1) << "Agent " << agent->getID() << " is going from state " << agent->getCurrentState()->getStateName() << " to " << state->getStateName();
#endif

	agent->setNextState(state);
	agent->setForceChangeState(true);
}
