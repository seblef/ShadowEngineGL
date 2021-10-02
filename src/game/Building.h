
#pragma once

#include "GameObject.h"
#include "BuildingTemplate.h"

class Map;

class Building
{
protected:

	int					_objectCount;
	GameObject**		_objects;
	Map*				_map;

public:

	Building(const BuildingTemplate& t, const Matrix4& world, Map& m);
	~Building();

	void							onAddToMap();
	void							onRemFromMap();
};
