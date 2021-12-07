
#pragma once

#include <string>

class Ground;
class Map;

class GroundBinLoader
{
public:

	GroundBinLoader()				{}

	void							loadGround(Ground& g, const std::string& groundFile, Map& mp) const;
};
