
#pragma once

#include <string>

class EntityTemplate;

namespace YAML
{
    class Node;
}

class EntityFactory
{
public:

	EntityFactory()				{}

	static EntityTemplate*		loadEntity(const std::string& className, const YAML::Node& node);
};
