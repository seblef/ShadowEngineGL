
#ifndef _BUILDING_H_
#define _BUILDING_H_

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

#endif