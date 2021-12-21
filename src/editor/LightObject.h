#pragma once

#include "Object.h"
#include "../renderer/Light.h"

class LightOmni;
class LightSpot;
class LightArea;

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
    void drawOmni(Drawer& drawer);
    void drawSpot(Drawer& drawer);
    void drawArea(Drawer& drawer);

public:
    LightObject();
    LightObject(const YAML::Node& node);
    LightObject(const LightObject& light);
    ~LightObject();

    const Light& getLight() const { return *_rLight; }
    const LightOmni& getOmni() const;
    const LightSpot& getSpot() const;
    const LightArea& getArea() const;

    void onAddToScene();
    void onRemFromScene();

    void drawSelected(Drawer& drawer);
};

}