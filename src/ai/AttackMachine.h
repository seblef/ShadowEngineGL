#pragma once

#include "AIMachine.h"
#include "Ids.h"

class AttackMachine : public AIMachine
{
public:

	AttackMachine();
	~AttackMachine()				{}

	const char*					getMachineName() const		{ return "attack"; }
	unsigned int				getStateID(const string& name);
};
