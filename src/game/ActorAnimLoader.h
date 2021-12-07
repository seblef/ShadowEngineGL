
#pragma once

#include "../core/AnimTracks.h"


class ActorAnimLoader
{
public:

	static Core::AnimPRSMultiTrack*			loadActorAnimation(const std::string& animFile);
};
