#include "EventPump.h"


void EventPump::postEvent(const Event& e)
{
    _events.push_back(e);
}

int	 EventPump::getEvent(Event& e)
{
    if(_events.size() > 0)
    {
        e=_events.front();
        _events.pop_front();
        return _events.size()+1;
    }
    else
        e._type=ET_COUNT;

    return _events.size();
}
