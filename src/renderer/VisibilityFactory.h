#pragma once

#include "IVisibilitySystem.h"
#include <string>

class VisibilityFactory
{
public:

	VisibilityFactory()				{}
	~VisibilityFactory()			{}

	static IVisibilitySystem*		createVisibilitySystem(const std::string& name);
};
