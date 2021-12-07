
#pragma once

#include <string>

class Actor;
class Material;
class IVideoDevice;

class ActorLoader
{
public:

	ActorLoader()				{}

	static Actor*				loadActor(const std::string& actorFile, Material* mat, IVideoDevice *device);
};
