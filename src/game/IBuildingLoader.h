
#pragma once

#include "BuildingTemplate.h"

class Map;

class IBuildingLoader
{
public:

	IBuildingLoader()			{}
	virtual ~IBuildingLoader()	{}

	virtual BuildingTemplate*	loadBuilding(const string& buildingName, Map& m)=0;
};
