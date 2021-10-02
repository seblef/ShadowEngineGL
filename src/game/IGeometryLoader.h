
#pragma once

#include "Geometry.h"
#include <string>

class IGeometryLoader
{
public:

	IGeometryLoader()						{}
	virtual ~IGeometryLoader()				{}

	virtual Geometry*						loadGeometry(const string& geoFileName)=0;
};
