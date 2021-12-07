#pragma once

#include "Event.h"
#include "../core/TSingleton.h"
#include <list>

using namespace std;

class EventPump : public Core::TSingleton<EventPump>
{
protected:

	typedef list<Event>		EventList;
	EventList				_events;

public:

	EventPump()				{}
	~EventPump()			{}

	void					postEvent(const Event& e);
	int						getEvent(Event& e);
};
