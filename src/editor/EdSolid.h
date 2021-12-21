#pragma once

#include "Object.h"
#include "../physic/PhysicDefs.h"
#include "../renderer/Mesh.h"
#include <memory>
#include <string>

class Mesh;
class MeshInstance;

namespace YAML
{
class Node;
}

namespace Editor
{

class EdGeometry;
class EdMaterial;

class EdSolidTemplate
{
protected:
    std::string _geometryName;
    std::string _materialName;

    Mesh _mesh;
    EdGeometry* _geometry;
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
    EdGeometry* getGeometry();

    void setGeometry(const std::string& geometry);
    void setMaterial(const std::string& material);
    void setShape(PhysicShape shape) { _shape = shape; }

    void load();
    bool isLoaded() const { return _loaded; }

    virtual void backup();
    virtual void restore();

};

class SolidObject : public Object
{
protected:
    EdSolidTemplate *_solidTemplate;
    MeshInstance* _meshInstance;

    void updateMatrix();

public:
    SolidObject(ObjectType type, EdSolidTemplate* solid);
    SolidObject(const SolidObject& solid);
    virtual ~SolidObject();

    void onAddToScene();
    void onRemFromScene();
};

}