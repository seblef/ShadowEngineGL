
#pragma once

#include "IMapLoader.h"

class MapTextLoader : public IMapLoader
{
protected:

	void parseMaterials(const YAML::Node& node);
	void parseGeometries(const YAML::Node& node);
	void parseGlobalLight(const YAML::Node& node);
	void parseMeshes(const YAML::Node& node);
	void parseParticles(const YAML::Node& node);
	void parseSounds(const YAML::Node& node);
	void parseBuildings(const YAML::Node& node);
	void parseObjects(const YAML::Node& node, Map& m);

	void parseMeshObject(const YAML::Node& node, Map& m);
	void parseParticlesObject(const YAML::Node& node, Map& m);
	void parseSoundObject(const YAML::Node& node, Map& m);
	void parseLight(const YAML::Node& node, Map& m);
	void parseActor(const YAML::Node& node, Map& m);
	void parseEntity(const YAML::Node& node, Map& m);
	void parseBuildingObject(const YAML::Node& node, Map& m);

	void					buildMatrix(const Vector3& rot, float x, float y, float height, Matrix4& world) const
	{
		Matrix4 rot_x, rot_y, rot_z;
		rot_x.createRotateX(rot.x * (float)M_PI / 180.0f);
		rot_y.createRotateY(rot.y * (float)M_PI / 180.0f);
		rot_z.createRotateZ(rot.z * (float)M_PI / 180.0f);

		world = rot_z*rot_y;
		world *= rot_x;

		world(3, 0) = x;
		world(3, 1) = height;
		world(3, 2) = y;
	}

public:

	MapTextLoader()					{}
	~MapTextLoader()				{}

	Map*					loadMap(const string& mapName);
};
