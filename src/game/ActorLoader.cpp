
#include "ActorLoader.h"
#include "ActorACTLoader.h"

Actor* ActorLoader::loadActor(const std::string& actorFile, Material* mat, IVideoDevice *device)
{
	IActorLoader *loader=0;
	Actor* a=0;
	std::string ext(actorFile.substr(actorFile.length()-3,3));
	
    if(ext=="act" || ext=="ACT")
		loader=new ActorACTLoader;

	if(loader)
	{
		a=loader->loadActor(actorFile,mat,device);
		delete loader;
	}

	return a;
}
