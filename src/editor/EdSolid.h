#pragma once

#include "../physic/PhysicDefs.h"
#include "../renderer/Mesh.h"
#include <memory>
#include <string>

class Mesh;

namespace YAML
{
class Node;
}


namespace Editor
{

class EdMaterial;

class EdSolidTemplate
{
protected:
    std::string _geometryName;
    std::string _materialName;

    Mesh _mesh;
    EdMaterial* _material;
    PhysicShape _shape;
    bool _loaded;

    EdSolidTemplate* _backup;

public:
    EdSolidTemplate();
    EdSolidTemplate(const YAML::Node& node);
    EdSolidTemplate(
        const std::string& geometry,
        const std::string& material
    );
    ~EdSolidTemplate();

    const std::string& getGeometryName() const { return _geometryName; }
    const std::string& getMaterialName() const { return _materialName; }
    PhysicShape getShape() const { return _shape; }

    const Mesh* getMesh();
    EdMaterial* getMaterial();

    void setGeometry(const std::string& geometry);
    void setMaterial(const std::string& material);
    void setShape(PhysicShape shape) { _shape = shape; }

    void load();
    bool isLoaded() const { return _loaded; }

    virtual void backup();
    virtual void restore();

};
}