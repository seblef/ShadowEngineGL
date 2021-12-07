#include "BSphere.h"

using namespace Core;

BSphere::BSphere() :
    _center(Vector3::NullVector),
    _radius(0)
{
}

BSphere::BSphere(const Vector3& center, float radius) :
    _center(center),
    _radius(radius)
{
}

BSphere::BSphere(const BSphere& b) :
    _center(b._center),
    _radius(b._radius)
{
}

BSphere::BSphere(const Vector3& min, const Vector3& max) :
    _center((min+max)*0.5f),
    _radius(((max-min) * 0.5f).getLength())
{
}

BSphere& BSphere::operator=(const BSphere& b)
{
    _center=b._center;
    _radius=b._radius;
    return *this;
}

const Vector3& BSphere::getCenter() const
{
    return _center;
}

float BSphere::getRadius() const
{
    return _radius;
}

void BSphere::setCenter(const Vector3& c)
{
    _center=c;
}

void		 BSphere::setRadius(float r)
{
    _radius=r;
}

float BSphere::getDistanceFrom(const Vector3& v) const
{
    return _center.getDistanceFrom(v) - _radius;
}

float BSphere::getDistanceFrom(const BSphere& b) const
{
    return getDistanceFrom(b._center) - b._radius;
}

BSphere& BSphere::operator << (const Vector3& v)
{
    float sq_len=(v - _center).getSquareLength();
    if(sq_len > _radius * _radius)
        _radius=sqrtf(sq_len);

    return *this;
}

BSphere& BSphere::operator << (const BSphere& b)
{
    float sq_len=(b._center - _center).getSquareLength() + b._radius * b._radius;
    if(sq_len > _radius * _radius)
        _radius=sqrtf(sq_len);

    return *this;
}

bool BSphere::isIn(const Vector3& p) const
{
    return getDistanceFrom(p) <= 0;
}

bool BSphere::isIn(const BSphere& b) const
{
    return getDistanceFrom(b) <= 0;
}

bool BSphere::isIn(const Vector3& min, const Vector3& max) const
{
    return _center.x + _radius >= min.x && _center.x - _radius <= max.x &&
            _center.y + _radius >= min.y && _center.y - _radius <= max.y &&
            _center.z + _radius >= min.z && _center.z - _radius <= max.z;
}

bool BSphere::isCompletlyIn(const BSphere& b) const
{
    return getDistanceFrom(b) <= -b._radius;
}

bool BSphere::isCompletlyIn(const Vector3& min, const Vector3& max) const
{
    return _center.x - _radius >= min.x && _center.x + _radius <= max.x &&
            _center.y - _radius >= min.y && _center.y + _radius <= max.y &&
            _center.z - _radius >= min.z && _center.z + _radius <= max.z;
}
