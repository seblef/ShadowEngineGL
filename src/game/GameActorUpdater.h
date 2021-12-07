
#pragma once

#include "../core/TSingleton.h"
#include <set>

class GameCharacter;

class GameActorUpdater : public Core::TSingleton<GameActorUpdater>
{
protected:

	typedef std::set<GameCharacter*>		ActorSet;
	ActorSet						_actors;

public:

	GameActorUpdater()				{}
	~GameActorUpdater()				{}

	void							addActor(GameCharacter* c);
	void							remActor(GameCharacter* c);

	void							updateActors(float time) const;
};
