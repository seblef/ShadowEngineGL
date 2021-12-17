#pragma once

#include "../core/BBox3.h"
#include "../core/Vector2.h"
#include <string>

namespace YAML
{
class Node;
}

namespace Editor
{

class EdGround;

class EdMap
{
protected:
    bool _valid;
    std::string _name;

    std::string _groundFile;
    EdGround* _ground;

    int _width, _height;
    Core::Vector2 _startPosition;

    void parseMapNode(const YAML::Node& node);
    void parseGlobalLightNode(const YAML::Node& node);
    void parseResourcesNode(const YAML::Node& node);
    void parseMaterialsNode(const YAML::Node& node);
    void parseGeometriesNode(const YAML::Node& node);
    void parseStaticsNode(const YAML::Node& node);

    void loadGround();

public:
    EdMap(const std::string& filename);
    ~EdMap();

    bool isValid() const { return _valid; }

    int getWidth() const { return _width; }
    int getHeight() const { return _height; }
};

}