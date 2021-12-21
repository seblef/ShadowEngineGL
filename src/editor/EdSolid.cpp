#include "EdSolid.h"
#include "EdGeometry.h"
#include "EdMaterial.h"
#include "Resources.h"
#include "Selection.h"
#include "../core/YAMLCore.h"
#include "../game/FilesStrings.h"
#include "../game/Geometry.h"
#include "../renderer/MeshInstance.h"
#include "../renderer/Renderer.h"
#include "../loguru.hpp"

namespace Editor
{

EdSolidTemplate::EdSolidTemplate() :
    _geometry(0),
    _material(0),
    _mesh(0, 0),
    _shape(PSHAPE_BOX),
    _loaded(false),
    _backup(0)
{
}

EdSolidTemplate::EdSolidTemplate(const YAML::Node& node) :
    _material(0),
    _geometry(0),
    _mesh(0, 0),
    _shape(PSHAPE_BOX),
    _loaded(false),
    _backup(0)
{
    _geometryName = node["geometry"].as<std::string>();
    _materialName = node["material"].as<std::string>();
    _shape = (PhysicShape)getPhysicShapeFromKey(
        node["physic_shape"].as<string>()
    );
}

EdSolidTemplate::EdSolidTemplate(
    const std::string& geometry,
    const std::string& material
) :
    _geometryName(geometry),
    _materialName(material),
    _material(0),
    _geometry(0),
    _mesh(0, 0),
    _shape(PSHAPE_BOX),
    _loaded(false),
    _backup(0)
{
}

EdSolidTemplate::~EdSolidTemplate()
{
    if(_backup)
        delete _backup;
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
    _geometry = (EdGeometry*)Resources::getSingletonRef().get(RES_GEOMETRY, _geometryName);
    _material = (EdMaterial*)Resources::getSingletonRef().get(RES_MATERIAL, _materialName);

    if(!_geometry)
    {
        LOG_S(ERROR) << "Failed loading geometry " << _geometryName;
        return;
    }
    if(!_material)
    {
        LOG_S(ERROR) << "Failed loading material " << _materialName;
        return;
    }

    _mesh.setGeometry(_geometry->getGeometry()->getRGeometry());
    _mesh.setMaterial(_material->getMaterial());
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

EdGeometry* EdSolidTemplate::getGeometry()
{
    if(!_loaded)
        load();
    return _geometry;
}

void EdSolidTemplate::backup()
{
    if(!_backup)
        _backup = new EdSolidTemplate();
    _backup->_geometryName = _geometryName;
    _backup->_materialName = _materialName;
    _backup->_shape = _shape;
}

void EdSolidTemplate::restore()
{
    if(!_backup)
        return;
    _geometryName = _backup->_geometryName;
    _materialName = _backup->_materialName;
    _shape = _backup->_shape;

    if(_loaded)
        load();
}


SolidObject::SolidObject(
    ObjectType type,
    EdSolidTemplate* solid
) :
    Object(type),
    _solidTemplate(solid),
    _meshInstance(0)
{
    _localBBox = _solidTemplate->getMesh()->getGeometry()->getBBox();
    _selectionActor = Selection::getSingletonRef().createMeshActor(
        *solid->getGeometry()->getSelectionGeometry()
    );
}

SolidObject::SolidObject(const SolidObject& solid) :
    Object(solid),
    _solidTemplate(solid._solidTemplate),
    _meshInstance(0)
{
    _selectionActor = Selection::getSingletonRef().createMeshActor(
        *solid._solidTemplate->getGeometry()->getSelectionGeometry()
    );
}

SolidObject::~SolidObject()
{
    if(_meshInstance)
        delete _meshInstance;
}

void SolidObject::updateMatrix()
{
    Object::updateMatrix();
    if(_meshInstance)
        _meshInstance->setWorldMatrix(_world);
}

void SolidObject::onAddToScene()
{
    _meshInstance = new MeshInstance(
        _solidTemplate->getMesh(),
        _world,
        false
    );
    Renderer::getSingletonRef().addRenderable(_meshInstance);
    Object::onAddToScene();
}

void SolidObject::onRemFromScene()
{
    if(!_meshInstance)
        return;
    Renderer::getSingletonRef().remRenderable(_meshInstance);
    delete _meshInstance;
    _meshInstance = 0;
    Object::onRemFromScene();
}

}