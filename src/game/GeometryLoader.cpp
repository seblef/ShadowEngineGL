
#include "GeometryLoader.h"
#include "Geo3DSLoader.h"
#include "GeoGEOLoader.h"
#include "../loguru.hpp"


Geometry* GeometryLoader::loadGeometry(const string& file)
{
	IGeometryLoader* loader=0;
	Geometry *geo=0;
	string ext(file.substr(file.length()-3,3));

    if(ext=="3ds" || ext=="3DS")
		loader=new Geo3DSLoader;
    else if(ext=="geo" || ext=="GEO")
		loader=new GeoGEOLoader;

	if(loader)
	{
		geo=loader->loadGeometry(file);
		delete loader;
	}
	else
		LOG_S(WARNING) << "[loadGeometry]: Unknown file format (" << file << ")";

	return geo;
}
