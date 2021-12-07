
#include "Light.h"
#include "LightSystem.h"
#include "../core/Quaternion.h"


Light::Light(LightType type, const LightCreate_t& c) : 
    Renderable(BBox3(-c._range,-c._range,-c._range,c._range,c._range,c._range),c._world,false),
    _type(type), _color(c._color), _range(c._range), _castShadows(c._castShadows), _shadowMapSize(c._shadowMapSize),
    _shadowMapFilter(c._shadowMapFilter), _direction(c._direction),	_shadowMap(0)
{
}

Light::Light(const Light *l) : Renderable(l), _type(l->_type), _color(l->_color), _range(l->_range),
    _castShadows(l->_castShadows), _shadowMapSize(l->_shadowMapSize),
    _shadowMapFilter(l->_shadowMapFilter), _direction(l->_direction), _shadowMap(0)
{
}

void Light::enqueue()
{
	LightSystem::getSingletonRef().enqueueLight(this);
}

void Light::updateWorldMatrix()
{
    Matrix4 w(_worldMatrix);
    computeMatrix(_direction,w);
    setWorldMatrix(w);
}

void Light::computeMatrix(const Vector3& d, Matrix4& m)
{
    Vector3 pos(m);

    Quaternion qRot;
    qRot.buildRotationArc(d,Vector3::ZAxisVector);
    qRot.getMatrix(m);
    m=pos;
}

void Light::setRange(float r)
{
    _range=r;
    _localBBox.setMin(Vector3(-r,-r,-r));
    _localBBox.setMax(Vector3(r,r,r));
    updateWorldMatrix();
}

void Light::setDirection(const Vector3& dir)
{
    _direction=dir;
    updateWorldMatrix();
}
