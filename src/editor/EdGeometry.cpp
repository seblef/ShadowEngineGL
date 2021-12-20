#include "EdGeometry.h"
#include "Selection.h"
#include "../game/GeometryLoader.h"

namespace Editor
{

EdGeometry::EdGeometry(const std::string& filename) :
    IResource(RES_GEOMETRY, filename),
    _selectionGeo(0)
{
    Geometry* geo = GeometryLoader::loadGeometry(filename);
    if(geo)
    {
        geo->buildRGeometry();
        _geometry = std::unique_ptr<Geometry>(geo);
        _selectionGeo = Selection::getSingletonRef().createMeshGeometry(*geo);
        _valid = true;
    }
}

EdGeometry::~EdGeometry()
{
    if(_selectionGeo)
        Selection::getSingletonRef().releaseMeshGeometry(_selectionGeo);
}

}