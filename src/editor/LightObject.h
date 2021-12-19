#pragma once

#include "Object.h"
#include "../renderer/Light.h"

namespace YAML
{
class Node;
}

namespace Editor
{

class LightObject : public Object
{
protected:
    Light::LightType _lightType;
    Light* _rLight;
    Core::Vector3 _initDirection;

    void updateMatrix();

public:
    LightObject();
    LightObject(const YAML::Node& node);
    LightObject(const LightObject& light);
    ~LightObject();

    void onAddToScene();
    void onRemFromScene();
};

}