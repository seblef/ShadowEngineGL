#include "EdGeometry.h"
#include "../game/GeometryLoader.h"

namespace Editor
{

EdGeometry::EdGeometry(const std::string& filename) :
    IResource(RES_GEOMETRY, filename),
    _valid(false)
{
    Geometry* geo = GeometryLoader::loadGeometry(filename);
    if(geo)
    {
        _geometry = std::unique_ptr<Geometry>(geo);
        _valid = true;
    }
}

}