#pragma once

#include "AIMachine.h"
#include "Ids.h"

class FollowMachine : public AIMachine
{
public:

	FollowMachine();
	~FollowMachine()				{}

	const char*					getMachineName() const		{ return "follow"; }
	unsigned int				getStateID(const string& name);
};
