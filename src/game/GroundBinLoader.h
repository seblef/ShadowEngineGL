
#pragma once

#include "Ground.h"
#include "Map.h"

class GroundBinLoader
{
public:

	GroundBinLoader()				{}

	void							loadGround(Ground& g, const string& groundFile, Map& mp) const;
};
