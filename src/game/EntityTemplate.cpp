
#include "EntityTemplate.h"
#include "FilesStrings.h"
#include "GameMaterial.h"
#include "GeometryLoader.h"
#include "../core/YAMLCore.h"
#include "../physic/Physic.h"
#include "../physic/PhysicGeometry.h"
#include "../renderer/Mesh.h"


EntityTemplate::EntityTemplate(bool isTrigger, const YAML::Node& node) :
	_rMaterial(0),
	_rMesh(0),
	_pGeometry(0),
	_geometry(0),
	_pShape(PSHAPE_SPHERE),
	_isTrigger(isTrigger),
	_meshName(node["mesh"].as<string>("")),
	_materialName(node["material"].as<string>(""))
{
	const string& s(node["shape"].as<string>(""));
    _pShape = (PhysicShape)getPhysicShapeFromKey(s);
}

void EntityTemplate::load()
{
	if (!_rMesh && !_rMaterial && !_meshName.empty() && !_materialName.empty())
	{
		_geometry = GeometryLoader::loadGeometry(_meshName);
        _geometry->buildRGeometry();
		assert(_geometry);
		_rMaterial = GameMaterial::loadMaterial(_materialName);
		assert(_rMaterial);

		_rMesh = new Mesh(_geometry->getRGeometry(),_rMaterial->getRMaterial());

		PhysicGeometryCreate_t c;
		c._box = _geometry->getBBox();
		c._shape = _pShape;
		c._tri = _geometry->getTriangles();
		c._triCount = _geometry->getTriangleCount();
		c._vx = _geometry->getVertices();
		c._vxCount = _geometry->getVertexCount();
		c._vxType = VX_3D;

		_pGeometry = Physic::getSingletonRef().createGeometry(c);
	}
}

void EntityTemplate::unload()
{
	if (_rMaterial)				delete _rMaterial;
	if (_rMesh)					delete _rMesh;
	if (_geometry)				delete _geometry;
	if (_pGeometry)				delete _pGeometry;

	_rMaterial = 0;
	_rMesh = 0;
	_geometry = 0;
	_pGeometry = 0;
}
