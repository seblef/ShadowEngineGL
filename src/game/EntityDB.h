
#pragma once

#include "EntityTemplate.h"
#include "../core/TDataBase.h"

class EntityDB : public Core::TDataBase<EntityTemplate>
{
public:

	EntityDB(const string& entityFile);

	void				unloadEntities();
};
