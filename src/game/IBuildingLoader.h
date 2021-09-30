
#ifndef _IBUILDINGLOADER_H_
#define _IBUILDINGLOADER_H_

#include "BuildingTemplate.h"

class Map;

class IBuildingLoader
{
public:

	IBuildingLoader()			{}
	virtual ~IBuildingLoader()	{}

	virtual BuildingTemplate*	loadBuilding(const string& buildingName, Map& m)=0;
};

#endif