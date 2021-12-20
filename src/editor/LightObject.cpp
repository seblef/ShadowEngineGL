#include "LightObject.h"
#include "Drawer.h"
#include "Selection.h"
#include "../game/TextParser.h"
#include "../renderer/LightArea.h"
#include "../renderer/LightOmni.h"
#include "../renderer/LightSpot.h"
#include "../renderer/Renderer.h"

namespace Editor
{

LightObject::LightObject() :
    Object(OBJ_LIGHT),
    _initDirection(Core::Vector3::ZAxisVector),
    _rLight(0)
{
    LightCreate_t create;
    create._color = Core::Color::White;
    create._range = 10.f;
    create._world = Matrix4::Identity;
    _rLight = new LightOmni(create);

    _selectionActor = Selection::getSingletonRef().createSphereActor(0.25f);
}

LightObject::LightObject(const YAML::Node& node) :
    Object(OBJ_LIGHT)
{
    LightCreate_t create;
    Light::LightType type;

    TextParser::parseLight(node, create, type);
    switch(type)
    {
    case Light::LT_OMNI:
        _rLight = new LightOmni(create);
        break;
    case Light::LT_SPOT:
        _rLight = new LightSpot(create);
        break;
    case Light::LT_AREA:
        _rLight = new LightArea(create);
        break;
    }
    _pos = create._world;
    _initDirection = create._direction;

    _selectionActor = Selection::getSingletonRef().createSphereActor(0.25f);
}

LightObject::LightObject(const LightObject& light) :
    Object(light),
    _initDirection(light._initDirection),
    _rLight(0)
{
    switch(light._rLight->getLightType())
    {
    case Light::LT_OMNI:
        _rLight = new LightOmni((LightOmni*)light._rLight);
        break;
    case Light::LT_SPOT:
        _rLight = new LightSpot((LightSpot*)light._rLight);
        break;
    case Light::LT_AREA:
        _rLight = new LightArea((LightArea*)light._rLight);
        break;
    }

    _selectionActor = Selection::getSingletonRef().createSphereActor(0.25f);
}

LightObject::~LightObject()
{
    if(_rLight)
        delete _rLight;
}

void LightObject::updateMatrix()
{
    if(!_onScene)
        return;

	if(_rLight->getLightType() == Light::LT_OMNI)
	{
		_world.createIdentity();
		_world = _pos;

		_rLight->setWorldMatrix(_world);
	}
	else
	{
        Object::updateMatrix();
		Matrix4 w(_world);

		Vector3 dir(_initDirection);
		dir.transformNoTranslation(w);

		_rLight->setDirection(dir);

		Light::computeMatrix(dir, w);
		_rLight->setWorldMatrix(w);
	}
}

void LightObject::onAddToScene()
{
    Object::onAddToScene();
    Renderer::getSingletonRef().addRenderable(_rLight);
    Drawer::getSingletonRef().addObject(this);
    updateMatrix();
}

void LightObject::onRemFromScene()
{
    Object::onRemFromScene();
    Drawer::getSingletonRef().remObject(this);
    Renderer::getSingletonRef().remRenderable(_rLight);
}

}