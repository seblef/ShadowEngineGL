
#pragma once

#include "EntityTemplate.h"

class EntityFactory
{
public:

	EntityFactory()				{}

	static EntityTemplate*		loadEntity(const string& className, const YAML::Node& node);
};
