
#pragma once

#include <vector>


class BuildingLight;
class BuildingMesh;


class BuildingTemplate
{
protected:

	typedef std::vector<BuildingMesh*>	MeshVector;
	typedef std::vector<BuildingLight*>	LightVector;

	MeshVector						_meshes;
	LightVector						_lights;

public:

	BuildingTemplate()				{}
	~BuildingTemplate();

	void							addMesh(BuildingMesh* m);
	void							addLight(BuildingLight* l);

	int								getMeshCount() const;
	int								getLightCount() const;

	BuildingMesh*					getMesh(int n) const;
	BuildingLight*					getLight(int n) const;
};
