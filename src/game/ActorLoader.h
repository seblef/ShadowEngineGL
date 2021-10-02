
#pragma once

#include "../StdRenderer.h"

class ActorLoader
{
public:

	ActorLoader()				{}

	static Actor*				loadActor(const string& actorFile, Material* mat, IVideoDevice *device);
};
