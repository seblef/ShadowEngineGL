
#include "AISystem.h"
#include "GazeMachine.h"
#include "FollowMachine.h"
#include "AttackMachine.h"
#include "../loguru.hpp"


AISystem::AISystem() : _currentAgentID(0), _currentMachineID(0), _now(0)
{
	_machines[GAZE_MACHINE_ID] = new GazeMachine;
	_machines[FOLLOW_MACHINE_ID] = new FollowMachine;
	_machines[ATTACK_MACHINE_ID] = new AttackMachine;
	//			[TODO] Add machines registration code
}

AISystem::~AISystem()
{
    for(const auto& a : _agents)
		delete a.second;

    for(const auto& m : _machines)
		delete m.second;
}

AIMachine* AISystem::getMachine(unsigned int machineID) const
{
	MachineMap::const_iterator m(_machines.find(machineID));
	return m == _machines.end() ? 0 : m->second;
}

unsigned int AISystem::getMachineID(const string& name) const
{
    for(const auto& m : _machines)
		if (name == m.second->getMachineName())
			return m.first;
	return 0;
}

unsigned int AISystem::createAgent(unsigned int machineID, unsigned int stateID,
	IAICallback* callback)
{
	AIMachine* m = getMachine(machineID);
	assert(m);
	AIMachineState* s = m->getState(stateID);
	assert(s);

	unsigned int id = _currentAgentID;
	AIAgent *a = new AIAgent(s, m, callback, _currentAgentID);
	++_currentAgentID;

	_agents[id]=a;
	return id;
}

void AISystem::destroyAgent(unsigned int agentID)
{
	AIAgent* a = getAgent(agentID);
	if (a)
		a->setCanDelete(true);
}

void AISystem::updateAI(const Vector3& playerPos, const Vector3& playerDir, float time)
{
	_now += (unsigned int)(time * 1000.0f);

	_playerPos = playerPos;
	_playerDir = playerDir;

	processEvents();

	AIAgent *agent;
    for(const auto& a : _agents)
	{
		agent = a.second;
		if (agent->getCanDelete())
			_destroyedAgents.push_back(agent->getID());
	}

	deleteDestroyedAgents();
}

void AISystem::deleteDestroyedAgents()
{
	AgentMap::iterator ait;
	UIntVector::const_iterator aid(_destroyedAgents.begin());
    for(const auto& aid : _destroyedAgents)
	{
		ait = _agents.find(aid);
		if (ait != _agents.end())
		{
			delete ait->second;
			_agents.erase(ait);
		}
	}
}

void AISystem::processEvents()
{
	//		[TODO] Adding delayed messages and removing them is not very efficient
    for(const auto& e : _delayedEvents)
		_events.push_back(e);
	_delayedEvents.clear();

    for(const auto& e : _events)
	{
		if (e.getDeliveryTime() <= _now)
			processSingleEvent(e);
		else
			_delayedEvents.push_back(e);
	}
	_events.clear();
}

void AISystem::processSingleEvent(const AIEvent& e)
{
	AIAgent *agent = getAgent(e.getRecieverID());
	if (!agent)
		return;		//	Agent doesn't exist anymore

	if (!agent->getCurrentState()->onEvent(agent, e))
		agent->getCurrentMachine()->onGlobalEvent(agent, e);

	while (agent->getForceStateChange())
	{
		AIEvent tempe(AIEVT_EXIT, agent->getID(), agent->getID());
		agent->setForceChangeState(false);
		agent->getCurrentState()->onEvent(agent,tempe);

		agent->setCurrentState(agent->getNextState());
		tempe.setType(AIEVT_ENTER);
		agent->getCurrentState()->onEvent(agent,tempe);
	}
}

AIAgent* AISystem::getAgent(unsigned int agentID) const
{
    const auto& a(_agents.find(agentID));
    return a == _agents.end() ? 0 : a->second;
}
