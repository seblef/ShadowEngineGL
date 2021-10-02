
#pragma once

#include "IGeometryLoader.h"

class GeoGEOLoader : public IGeometryLoader
{
public:

	GeoGEOLoader()					{}
	~GeoGEOLoader()					{}

	Geometry*						loadGeometry(const string& geoFile);
};
