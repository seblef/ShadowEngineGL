
#pragma once

#include "IMapLoader.h"

class MapLoaderFactory
{
public:

	MapLoaderFactory()				{}

	static IMapLoader*				createMapLoader(const string& loaderName);
};
