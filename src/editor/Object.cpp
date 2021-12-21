#include "Object.h"
#include "Drawer.h"
#include "Selection.h"
#include "../physic/PhysicUtils.h"
#include <PxPhysicsAPI.h>


namespace Editor
{

Object::Object(ObjectType type) :
    _type(type),
    _pos(Core::Vector3::NullVector),
    _rot(Core::Vector3::NullVector),
    _world(Core::Matrix4::Identity),
    _localBBox(Core::BBox3::NullBox),
    _worldBBox(Core::BBox3::NullBox),
    _selectionActor(0),
    _onScene(false)
{
}

Object::Object(const Object& obj) :
    _type(obj._type),
    _name(obj._name),
    _pos(obj._pos),
    _rot(obj._rot),
    _world(obj._world),
    _localBBox(obj._localBBox),
    _worldBBox(obj._worldBBox),
    _selectionActor(0),
    _onScene(false)
{
}

Object::~Object()
{
}

void Object::setPosition(const Core::Vector3& pos)
{
    _pos = pos;
    updateMatrix();
}

void Object::translate(const Core::Vector3& translation)
{
    _pos += translation;
    updateMatrix();
}

void Object::setRotation(const Core::Vector3& rot)
{
    _rot = rot;
    updateMatrix();
}

void Object::rotate(const Core::Vector3& rotation)
{
    _rot += rotation;
    updateMatrix();
}

void Object::updateMatrix()
{
	Core::Matrix4 rot_x,rot_y,rot_z;
	rot_x.createRotateX(_rot.x * M_PI / 180.0f);
	rot_y.createRotateY(_rot.y * M_PI / 180.0f);
	rot_z.createRotateZ(_rot.z * M_PI / 180.0f);

	_world = rot_z*rot_y;
	_world *= rot_x;
	_world = _pos;

    _worldBBox = _localBBox * _world;

    if(_onScene)
        setActorMatrix();
}

void Object::onAddToScene()
{
    _selectionActor->userData = this;
    Selection::getSingletonRef().addActorToScene(*_selectionActor);
    setActorMatrix();
    _onScene = true;
}

void Object::onRemFromScene()
{
    Selection::getSingletonRef().remActorFromScene(*_selectionActor);
    _onScene = false;
}

void Object::setActorMatrix()
{
    PMAKETRANSFORM(t, _world);
    _selectionActor->setKinematicTarget(t);
}

void Object::drawSelected(Drawer& drawer)
{
    drawer.setColor(Core::Color(.0f, .8f, .8f, .0f));
    drawer.setWorldMatrix(_world);
    drawer.drawBox(_localBBox.getMin(), _localBBox.getMax());
}

}