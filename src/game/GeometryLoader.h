
#pragma once

#include "Geometry.h"

class GeometryLoader
{
public:

	GeometryLoader()					{}

	static Geometry*					loadGeometry(const string& geoFileName);
};
