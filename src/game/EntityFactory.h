
#pragma once

#include "EntityTemplate.h"

class EntityFactory
{
public:

	EntityFactory()				{}

	static EntityTemplate*		loadEntity(const string& className, ScriptFile& sf);
};
