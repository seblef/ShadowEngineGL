#pragma once

#include "IVisibilitySystem.h"

class VisibilityFactory
{
public:

	VisibilityFactory()				{}
	~VisibilityFactory()			{}

	static IVisibilitySystem*		createVisibilitySystem(const string& name);
};
