#pragma once

#include "IResource.h"
#include "../renderer/Material.h"
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

class EdMaterialSave
{
protected:
    Material* _material;
    MaterialStdBuffer _buffer;
    std::string _textures[TS_COUNT];
    unsigned int _flags;
    bool _blendEnable;
    BlendMode _srcBlend;
    BlendMode _destBlend;
    CullMode _cull;
    TVector<MaterialAnimation> _animations;

public:
    EdMaterialSave(Material* material);

    void restore();
};

}