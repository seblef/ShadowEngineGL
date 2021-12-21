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

const float LightColorAttenuation = .5f;

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

const LightOmni& LightObject::getOmni() const
{
    return *((LightOmni*)_rLight);
}

const LightSpot& LightObject::getSpot() const
{
    return *((LightSpot*)_rLight);
}

const LightArea& LightObject::getArea() const
{
    return *((LightArea*)_rLight);
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

void LightObject::drawSelected(Drawer& drawer)
{
    drawer.setColor(_rLight->getColor());
    drawer.setWorldMatrix(_rLight->getWorldMatrix());
    
    switch (_rLight->getLightType())
    {
    case Light::LT_OMNI:
        drawOmni(drawer);
        break;
    case Light::LT_SPOT:
        drawSpot(drawer);
        break;
    case Light::LT_AREA:
        drawArea(drawer);
        break;
    }
}

void LightObject::drawOmni(Drawer& drawer)
{
	const float range = _rLight->getRange() * 0.5f;
    drawer.drawCircle(
        Core::Vector3::NullVector,
        Core::Vector3::XAxisVector,
        range
    );
    drawer.drawCircle(
        Core::Vector3::NullVector,
        Core::Vector3::YAxisVector,
        range
    );
    drawer.drawCircle(
        Core::Vector3::NullVector,
        Core::Vector3::ZAxisVector,
        range
    );
}

void LightObject::drawSpot(Drawer& drawer)
{
	const float range = _rLight->getRange();
	float angle = getSpot().getFarAngle() * M_PI / 360.0f;
	float sin_angle = sinf(angle);
	float cos_angle = cosf(angle);
	float scale = range * sin_angle / cos_angle;

	drawer.setColor(_rLight->getColor() * LightColorAttenuation);
	drawer.drawCircle(
        Core::Vector3(.0f,.0f,range),
        Vector3::ZAxisVector,
        scale
    );

	drawer.drawLine(Core::Vector3::NullVector, Core::Vector3(-scale,0.0f,range));
	drawer.drawLine(Core::Vector3::NullVector, Core::Vector3( scale,0.0f,range));
	drawer.drawLine(Core::Vector3::NullVector, Core::Vector3(0.0f,-scale,range));
	drawer.drawLine(Core::Vector3::NullVector, Core::Vector3(0.0f, scale,range));

	angle = getSpot().getNearAngle() * M_PI / 360.0f;
	sin_angle = sinf(angle);
	cos_angle = cosf(angle);
	scale = range * sin_angle / cos_angle;

	drawer.setColor(_rLight->getColor());
	drawer.drawCircle(
        Core::Vector3(0.0f,0.0f,range),
        Core::Vector3::ZAxisVector,
        scale
    );

	drawer.drawLine(Core::Vector3::NullVector, Core::Vector3(-scale,0.0f,range));
	drawer.drawLine(Core::Vector3::NullVector, Core::Vector3( scale,0.0f,range));
	drawer.drawLine(Core::Vector3::NullVector, Core::Vector3(0.0f,-scale,range));
	drawer.drawLine(Core::Vector3::NullVector, Core::Vector3(0.0f, scale,range));
}

void LightObject::drawArea(Drawer& drawer)
{
	const float range = _rLight->getRange();
	const float width = getArea().getAreaWidth() * 0.5f;
	const float height = getArea().getAreaHeight() * 0.5f;
	const float aspect = width / height;
	float xscale,yscale;
	float angle = getArea().getFarAngle() * M_PI / 360.0f;
	float sin_angle = sinf(angle);
	float cos_angle = cosf(angle);
	float div = cos_angle / sin_angle;

	yscale = 0.5f * (range) / div + height;
	xscale = yscale * aspect + width;

	drawer.drawRect(
        Core::Vector3(-width,-height,0.0f),
        Core::Vector3( width,-height,0.0f),
        Core::Vector3( width, height,0.0f),
        Core::Vector3(-width, height,0.0f)
    );
	drawer.setColor(_rLight->getColor() * LightColorAttenuation);
	drawer.drawRect(
        Core::Vector3(-xscale,-yscale,range),
		Core::Vector3( xscale,-yscale,range),
		Core::Vector3( xscale, yscale,range),
		Core::Vector3(-xscale, yscale,range)
    );
	drawer.drawLine(Core::Vector3(-width,-height,0.0f), Core::Vector3(-xscale,-yscale,range));
	drawer.drawLine(Core::Vector3( width,-height,0.0f), Core::Vector3( xscale,-yscale,range));
	drawer.drawLine(Core::Vector3( width, height,0.0f), Core::Vector3( xscale, yscale,range));
	drawer.drawLine(Core::Vector3(-width, height,0.0f), Core::Vector3(-xscale, yscale,range));

	angle = getSpot().getNearAngle() * M_PI / 360.0f;
	sin_angle = sinf(angle);
	cos_angle = cosf(angle);
	div = cos_angle / sin_angle;

	yscale = 0.5f * (range) / div + height;
	xscale = yscale * aspect + width;

	drawer.setColor(_rLight->getColor());
	drawer.drawRect(
        Core::Vector3(-xscale,-yscale,range),
		Core::Vector3( xscale,-yscale,range),
		Core::Vector3( xscale, yscale,range),
		Core::Vector3(-xscale, yscale,range)
    );
	drawer.drawLine(Core::Vector3(-width,-height,0.0f), Core::Vector3(-xscale,-yscale,range));
	drawer.drawLine(Core::Vector3( width,-height,0.0f), Core::Vector3( xscale,-yscale,range));
	drawer.drawLine(Core::Vector3( width, height,0.0f), Core::Vector3( xscale, yscale,range));
	drawer.drawLine(Core::Vector3(-width, height,0.0f), Core::Vector3(-xscale, yscale,range));
}

}