#pragma once

#include "../core/BBox3.h"
#include "../core/Matrix4.h"
#include "../core/Vector3.h"
#include <string>

namespace Editor
{

enum ObjectType
{
    OBJ_STATIC=0,
    OBJ_LIGHT,

    OBJ_COUNT
};

class Object
{
private:
    ObjectType _type;

protected:
    std::string _name;
    Core::Vector3 _pos;
    Core::Vector3 _rot;
    Core::Matrix4 _world;
    Core::BBox3 _localBBox;
    Core::BBox3 _worldBBox;
    bool _onScene;

    virtual void updateMatrix();

public:
    Object(ObjectType type);
    Object(const Object& obj);
    virtual ~Object();

    ObjectType getType() const { return _type; }

    const std::string& getName() const { return _name; }
    void setName(const std::string& name) { _name = name; }

    const Core::Vector3& getPosition() const { return _pos; }
    void setPosition(const Core::Vector3& pos);
    void translate(const Core::Vector3& translation);

    const Core::Vector3& getRotation() const { return _rot; }
    void setRotation(const Core::Vector3& rot);
    void rotate(const Core::Vector3& rotation);

    const Core::Matrix4& getWorldMatrix() const { return _world; }
    const Core::BBox3& getLocalBBox() const { return _localBBox; }
    const Core::BBox3& getWorldBBox() const { return _worldBBox; }

    virtual void onAddToScene();
    virtual void onRemFromScene();
};

}