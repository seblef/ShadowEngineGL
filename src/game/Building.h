
#pragma once

class BuildingTemplate;
class GameObject;
class Map;

namespace Core
{
    class Matrix4;
}

class Building
{
protected:

	int					_objectCount;
	GameObject**		_objects;
	Map*				_map;

public:

	Building(const BuildingTemplate& t, const Core::Matrix4& world, Map& m);
	~Building();

	void							onAddToMap();
	void							onRemFromMap();
};
