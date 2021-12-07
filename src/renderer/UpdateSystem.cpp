#include "UpdateSystem.h"
#include "IUpdatable.h"


void UpdateSystem::registerUpdatable(IUpdatable* u)
{
    _updatables.insert(u);
}

void UpdateSystem::unregisterUpdatable(IUpdatable* u)
{
    _updatables.erase(u);
}

void UpdateSystem::process(float time) const
{
    for(auto const& u : _updatables)
        u->update(time);
}
