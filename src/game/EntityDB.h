
#pragma once

#include "EntityTemplate.h"

class EntityDB : public TDataBase<EntityTemplate>
{
public:

	EntityDB(const string& entityFile);

	void				unloadEntities();
};
