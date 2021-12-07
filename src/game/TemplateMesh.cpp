
#include "TemplateMesh.h"
#include "GameMaterial.h"
#include "ObjectFlags.h"
#include "GeometryLoader.h"
#include "TextParser.h"
#include "ResourceDB.h"
#include "../physic/Physic.h"
#include "../physic/PhysicGeometry.h"
#include "../renderer/Mesh.h"


TemplateMesh::TemplateMesh(Geometry* geo, GameMaterial *gmat, unsigned int flags,
	PhysicShape shape, bool deleteRes) : _geometry(geo), _flags(flags), _physic(0), _gameMaterial(gmat), _deleteRes(deleteRes)
{
	_rMesh=new Mesh(geo->getRGeometry(),gmat->getRMaterial());

	if((flags & OF_NOCOLLISION)==0)
	{
		PhysicGeometryCreate_t c;
		c._box=geo->getBBox();
		c._shape=shape;
		c._tri=geo->getTriangles();
		c._triCount=geo->getTriangleCount();
		c._vx=geo->getVertices();
		c._vxCount=geo->getVertexCount();
		c._vxType=VX_3D;

		_physic=Physic::getSingletonRef().createGeometry(c);
	}
}

TemplateMesh::~TemplateMesh()
{
	delete _rMesh;
	if(_physic)			delete _physic;

	if (_deleteRes)
	{
		delete _gameMaterial;
		delete _geometry;
	}
}

TemplateMesh* TemplateMesh::loadTemplate(const string& geoFile, const string& matFile, unsigned int flags, PhysicShape shape)
{
	Geometry *geo = GeometryLoader::loadGeometry(geoFile);
	GameMaterial *m = GameMaterial::loadMaterial(matFile);
	geo->buildRGeometry();

	return new TemplateMesh(geo, m, flags, shape, true);
}
