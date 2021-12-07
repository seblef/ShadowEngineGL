
#pragma once

#include <string>

class Geometry;

class IGeometryLoader
{
public:

	IGeometryLoader()						{}
	virtual ~IGeometryLoader()				{}

	virtual Geometry*						loadGeometry(const std::string& geoFileName)=0;
};
