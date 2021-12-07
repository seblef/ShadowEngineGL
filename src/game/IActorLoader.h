
#pragma once

#include <string>

class Actor;
class Material;
class IVideoDevice;

class IActorLoader
{
public:

	IActorLoader()				{}
	virtual ~IActorLoader()		{}

	virtual Actor*				loadActor(const std::string& actorFile, Material* mat, IVideoDevice *device)=0;
};
