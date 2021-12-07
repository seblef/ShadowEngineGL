#include "ActionServer.h"
#include "Action.h"
#include "../loguru.hpp"

ActionServer::~ActionServer()
{
	reset();
}

void ActionServer::reset()
{
    for(auto const& a : _activeActions)
		delete a;

	_activeActions.clear();
}

void ActionServer::addAction(Action *a)
{
#ifdef _DEBUG
	LOG_S(1) << "Queueing action" << a->getName();
#endif
	_activeActions.insert(a);
}

void ActionServer::addDelayedAction(Action *a, float delay)
{
	assert(delay > 0.0f);
#ifdef _DEBUG
	LOG_S(1) << "Queueing delayed action" << a->getName() << ". Delay: " << delay;
#endif
	a->getDelayTime() = delay;
	_delayedActions.insert(a);
}

void ActionServer::update(float time)
{
	Action *action;

    for(auto const& a : _delayedActions)
	{
		a->getDelayTime() -= time;
		if (a->getDelayTime() < 0.0f)
		{
#ifdef _DEBUG
			LOG_S(1) << "Running delayed action " << a->getName();
#endif
			_activeActions.insert(a);
			_deadActions.push_back(a);
		}
	}

    for(auto const& da : _deadActions)
		_delayedActions.erase(da);

	_deadActions.clear();

	for(auto const& a : _activeActions)
	{
		a->update(time);
		if (a->isDead())
		{
#ifdef _DEBUG
			LOG_S(1) << "Action " << a->getName() << " is finished, deleting it";
#endif
			_deadActions.push_back(a);
		}
	}

    for(auto const& da : _deadActions)
	{
		_activeActions.erase(da);
		delete da;
	}

	_deadActions.clear();
}
