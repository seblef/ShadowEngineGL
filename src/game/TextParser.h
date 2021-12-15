
#pragma once

#include "../renderer/Light.h"

class Material;
class GameMaterial;
class TemplateMesh;

namespace Particles
{
class SystemTemplate;
}

namespace YAML
{
    class Node;
}

class TextParser
{
protected:


public:

	TextParser()			{}
	~TextParser()			{}

	static GameMaterial* parseMaterial(
        const YAML::Node& node,
        const std::string& name
    );
	static TemplateMesh* parseMesh(
        const YAML::Node& node,
        const std::string& name,
        const std::string* geoDir = 0
    );
	static Particles::SystemTemplate* parseParticles(
        const YAML::Node& node,
        const std::string& name
    );
	static void parseLight(
        const YAML::Node& node,
        LightCreate_t& lc,
        Light::LightType& t
    );
};


Material* parseRendererMaterial(const YAML::Node& node);
