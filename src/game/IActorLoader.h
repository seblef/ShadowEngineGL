
#pragma once

#include "../StdRenderer.h"

class IActorLoader
{
public:

	IActorLoader()				{}
	virtual ~IActorLoader()		{}

	virtual Actor*				loadActor(const string& actorFile, Material* mat, IVideoDevice *device)=0;
};
