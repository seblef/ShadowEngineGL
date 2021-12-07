
#pragma once

#include <string>

class BuildingTemplate;
class Map;

class IBuildingLoader
{
public:

	IBuildingLoader()			{}
	virtual ~IBuildingLoader()	{}

	virtual BuildingTemplate*	loadBuilding(const std::string& buildingName, Map& m)=0;
};
