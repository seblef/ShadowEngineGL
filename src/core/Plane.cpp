#include "Plane.h"
#include "MathUtils.h"
#include "CoreCommon.h"
#include "BSphere.h"


using namespace Core;


Plane::Plane(const Plane& p) :
    _normal(p._normal),
    _dist(p._dist)
{
}

Plane::Plane(const Vector3& normal, float dist) :
    _normal(normal),
    _dist(dist)
{
}

Plane::Plane(const Vector3& point, const Vector3& norm) :
    _normal(norm)
{
    recalculateDist(point);
}

Plane::Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
    makePlane(p1,p2,p3);
}

Plane& Plane::operator=(const Plane& p)
{
    _normal=p._normal;
    _dist=p._dist;
    return *this;
}

const Vector3& Plane::getNormal() const
{
    return _normal;
}

float Plane::getDistance() const
{
    return _dist;
}

void Plane::recalculateDist(const Vector3& v)
{
    _dist=-(v * _normal);
}

void Plane::makePlane(const Vector3& normal, float dist)
{
    _normal=normal; _dist=dist;
}

void Plane::makePlane(const Vector3& point, const Vector3& norm)
{
    _normal=norm;
    recalculateDist(point);
}

void Plane::makePlane(const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
    _normal=(p2-p1) ^ (p3-p1);
    _normal.normalize();
    recalculateDist(p1);
}

float Plane::getDistanceFrom(const Vector3& p) const
{
    return _normal * p + _dist;
}

bool Plane::intersectLine(const Vector3& o, const Vector3& d, float& t) const
{
    float t2=_normal * d;
    if(t2==0)
    {
        t=WBOUNDS_P;
        return false;
    }
    else
    {
        t=-(_normal * o + _dist) / t2;
        return true;
    }
}
bool Plane::intersectLine(const Vector3& o, const Vector3& d, Vector3& p) const
{
    float t2=_normal * d;
    if(t2==0)
        return false;

    float t=-(_normal * o + _dist) / t2;
    p=o + (d*t);
    return true;
}

bool Plane::intersectEdge(const Vector3& v1, const Vector3& v2, Vector3& p) const
{
    Vector3 d(v2-v1);
    float t;
    if(intersectLine(v1,d,t) && t <= 1.0f  && t >= 0.0f)
    {
        p=v1 + d*t;
        return true;
    }
    else
        return false;
}

PlaneSide Plane::classifyPoint(const Vector3& p) const
{
    float d=getDistanceFrom(p);
    if(d < -SEPSILON)		return PLANE_BACK;
    else if(d > SEPSILON)	return PLANE_FRONT;
    else					return PLANE_ON;
}

PlaneSide Plane::classifySphere(const BSphere& b) const
{
    float d=getDistanceFrom(b.getCenter());
    if(d < -b.getRadius() - SEPSILON)		return PLANE_BACK;
    else if(d > b.getRadius() + SEPSILON)	return PLANE_FRONT;
    else									return PLANE_CLIP;
}

PlaneSide Plane::classifyBox(const BBox3& b) const
{
    Vector3 near_(b.getMax());
    Vector3 far_(b.getMin());

    if(_normal.x > 0.0f)	sswap(near_.x,far_.x);
    if(_normal.y > 0.0f)	sswap(near_.y,far_.y);
    if(_normal.z > 0.0f)	sswap(near_.z,far_.z);

    if(getDistanceFrom(near_) > 0.0f)		return PLANE_FRONT;
    else if(getDistanceFrom(far_) > 0.0f)	return PLANE_CLIP;
    else									return PLANE_BACK;
}

Vector3 Plane::getPoint() const
{
    return _normal * (-_dist);
}

void Plane::getPoint(Vector3& p) const
{
    p=_normal * (-_dist);
}

bool Plane::existsPlaneIntersection(const Plane& p) const
{
    return (p.getNormal() ^ _normal).getSquareLength() > (SEPSILON * SEPSILON);
}

bool Plane::intersectPlane(const Plane& p, Vector3& o, Vector3& d) const
{
    float fn00=_normal.getLength();
    float fn01=_normal * p._normal;
    float fn11=p._normal.getLength();
    float det=fn00*fn11 - fn01*fn01;

    if(FloatMakePositive(det) < SEPSILON)
        return false;
    else
    {
        det=1.0f / det;
        float fc0=(fn11 * (-_dist) + fn01 * p._dist) * det;
        float fc1=(fn00 * (-p._dist) + fn01 * _dist) * det;

        d=_normal ^ p._normal;
        o=_normal * fc0 + p._normal * fc1;

        return true;
    }
}

bool Plane::intersectWithPlanes(const Plane& p1, const Plane& p2, Vector3& it) const
{
    Vector3 line_pt,line_dir;
    if(intersectPlane(p1,line_pt,line_dir))
        return p2.intersectLine(line_pt, line_dir,it);
    else
        return false;
}

bool Plane::isFront(const Vector3& look_dir) const
{
    return (_normal * look_dir) <= 0.0f;
}

Plane Plane::operator*(const Matrix4& mat) const
{
    Vector3 m(getPoint() * mat);
    Vector3 norm((_normal * mat) - (const Vector3&)mat);
    norm.normalize();

    return Plane(norm,_dist - (m * norm));
}

Plane& Plane::operator*=(const Matrix4& mat)
{
    Vector3 m(getPoint() * mat);
    _normal=(_normal * mat) - (const Vector3&)mat;
    _normal.normalize();
    _dist-=(m*_normal);
    return *this;
}