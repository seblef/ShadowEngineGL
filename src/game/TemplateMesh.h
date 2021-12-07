
#pragma once

#include "../physic/PhysicDefs.h"

class GameMaterial;
class Geometry;
class PhysicGeometry;
class Mesh;

class TemplateMesh
{
protected:

	Geometry*					_geometry;
	GameMaterial*				_gameMaterial;
	Mesh*						_rMesh;
	unsigned int				_flags;

	PhysicGeometry*				_physic;

	bool						_deleteRes;

public:

	TemplateMesh(Geometry* geo, GameMaterial *gmat, unsigned int flags, PhysicShape shape, bool deleteRes);
	~TemplateMesh();

	unsigned int				getFlags() const					{ return _flags; }
	const Geometry&				getGeometry() const					{ return *_geometry; }
	Mesh*						getMesh() const						{ return _rMesh; }
	PhysicGeometry*				getPhysicGeometry() const			{ return _physic; }
	GameMaterial*				getGameMaterial() const				{ return _gameMaterial; }

	static TemplateMesh*		loadTemplate(const string& geoFile, const string& matFile, unsigned int flags, PhysicShape shape);
};
