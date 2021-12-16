#pragma once

#include "../physic/PhysicDefs.h"
#include "../renderer/Mesh.h"
#include <memory>
#include <string>

class Mesh;

namespace Editor
{

class EdMaterial;

class EdSolidTemplate
{
protected:
    std::string _name;
    std::string _geometryName;
    std::string _materialName;

    Mesh _mesh;
    EdMaterial* _material;
    PhysicShape _shape;
    bool _loaded;

public:
    EdSolidTemplate();
    EdSolidTemplate(
        const std::string& name,
        const std::string& geometry,
        const std::string& material
    );

    const std::string& getName() const { return _name; }
    const std::string& getGeometryName() const { return _geometryName; }
    const std::string& getMaterialName() const { return _materialName; }
    PhysicShape getShape() const { return _shape; }

    const Mesh* getMesh();
    EdMaterial* getMaterial();

    void setName(const std::string& name) { _name = name; }
    void setGeometry(const std::string& geometry);
    void setMaterial(const std::string& material);
    void setShape(PhysicShape shape) { _shape = shape; }

    void load();
    bool isLoaded() const { return _loaded; }

};
}