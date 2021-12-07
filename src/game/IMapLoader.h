
#pragma once

#include <string>

class Map;


class IMapLoader
{
public:

	IMapLoader()				{}
	virtual ~IMapLoader()		{}

	virtual Map*				loadMap(const std::string& mapName)=0;
};
