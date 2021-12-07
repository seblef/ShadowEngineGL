
#pragma once

#include "AIMachine.h"
#include "Ids.h"

class GazeMachine : public AIMachine
{
public:

	GazeMachine();
	~GazeMachine()				{}

	const char*					getMachineName() const		{ return "gaze"; }
	unsigned int				getStateID(const string& name);
};
