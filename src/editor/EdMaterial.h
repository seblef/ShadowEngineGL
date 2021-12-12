#pragma once

#include "IResource.h"
#include <string>

class Material;

namespace YAML
{
class Node;
}

namespace Editor
{

class EdMaterial : public IResource
{
protected:
    std::string _debrisMesh;
    Material *_material;
    bool _valid;

    void parse(const YAML::Node& node);

public:
    EdMaterial();
    EdMaterial(const std::string& filename);
    EdMaterial(const std::string& name, const YAML::Node& node);
    ~EdMaterial();

    const std::string& getDebrisMesh() const { return _debrisMesh; }
    void setDebrisMesh(const std::string& debrisMesh) { _debrisMesh = debrisMesh; }

    Material* getMaterial() const { return _material; }

    bool isValid() const { return _valid; }
};

}