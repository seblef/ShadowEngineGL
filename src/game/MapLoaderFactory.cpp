
#include "MapLoaderFactory.h"
#include "MapTextLoader.h"

IMapLoader* MapLoaderFactory::createMapLoader(const std::string& name)
{
	return new MapTextLoader;
}
