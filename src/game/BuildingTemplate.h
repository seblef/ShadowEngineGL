
#pragma once

#include "BuildingMesh.h"
#include "BuildingLight.h"


class BuildingTemplate
{
protected:

	typedef vector<BuildingMesh*>	MeshVector;
	typedef vector<BuildingLight*>	LightVector;

	MeshVector						_meshes;
	LightVector						_lights;

public:

	BuildingTemplate()				{}
	~BuildingTemplate();

	void							addMesh(BuildingMesh* m)			{ _meshes.push_back(m); }
	void							addLight(BuildingLight* l)			{ _lights.push_back(l); }

	int								getMeshCount() const				{ return _meshes.size(); }
	int								getLightCount() const				{ return _lights.size(); }

	BuildingMesh*					getMesh(int n) const				{ return _meshes[n]; }
	BuildingLight*					getLight(int n) const				{ return _lights[n]; }
};
