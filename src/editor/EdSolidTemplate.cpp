#include "EdSolidTemplate.h"
#include "EdGeometry.h"
#include "EdMaterial.h"
#include "Resources.h"
#include "../game/Geometry.h"
#include "../loguru.hpp"

namespace Editor
{

EdSolidTemplate::EdSolidTemplate() :
    _material(0),
    _mesh(0, 0),
    _shape(PSHAPE_BOX),
    _loaded(false)
{
}

EdSolidTemplate::EdSolidTemplate(
    const std::string& name,
    const std::string& geometry,
    const std::string& material
) :
    _name(name),
    _geometryName(geometry),
    _materialName(material),
    _material(0),
    _mesh(0, 0),
    _shape(PSHAPE_BOX),
    _loaded(false)
{
}

void EdSolidTemplate::setGeometry(const std::string& geometry)
{
    _geometryName = geometry;
    if(_loaded)
        load();
}

void EdSolidTemplate::setMaterial(const std::string& material)
{
    _materialName = material;
    if(_loaded)
        load();
}

void EdSolidTemplate::load()
{
    EdGeometry* geo = (EdGeometry*)Resources::getSingletonRef().get(RES_GEOMETRY, _geometryName);
    EdMaterial* mat = (EdMaterial*)Resources::getSingletonRef().get(RES_MATERIAL, _materialName);

    if(!geo)
    {
        LOG_S(ERROR) << "Failed loading geometry " << _geometryName;
        return;
    }
    if(!mat)
    {
        LOG_S(ERROR) << "Failed loading material " << _materialName;
        return;
    }

    _mesh.setGeometry(geo->getGeometry()->getRGeometry());
    _mesh.setMaterial(mat->getMaterial());
    _loaded = true;
}

const Mesh* EdSolidTemplate::getMesh()
{
    if(!_loaded)
        load();
    return &_mesh;
}

EdMaterial* EdSolidTemplate::getMaterial()
{
    if(!_loaded)
        load();
    return _material;
}

}