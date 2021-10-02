
#pragma once

#include "Map.h"
#include <string>


class IMapLoader
{
public:

	IMapLoader()				{}
	virtual ~IMapLoader()		{}

	virtual Map*				loadMap(const string& mapName)=0;
};
