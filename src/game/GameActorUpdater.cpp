#include "GameActorUpdater.h"
#include "GameCharacter.h"


void GameActorUpdater::addActor(GameCharacter* c)
{
    _actors.insert(c);
}

void GameActorUpdater::remActor(GameCharacter* c)
{
    _actors.erase(c);
}

void GameActorUpdater::updateActors(float time) const
{
    for(auto const& a : _actors)
        a->update(time);
}
