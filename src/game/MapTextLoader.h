
#pragma once

#include "IMapLoader.h"

namespace YAML
{
    class Node;
}

namespace Core
{
    class Vector3;
    class Matrix4;
}

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

	void					buildMatrix(const Core::Vector3& rot, float x, float y, float height, Core::Matrix4& world) const;

public:

	MapTextLoader()					{}
	~MapTextLoader()				{}

	Map*					loadMap(const std::string& mapName);
};
