
#include "BBox2.h"
#include "BBox3.h"
#include "BSphere.h"
#include "CoreCommon.h"

using namespace Core;

BBox2	BBox2::NullBox(0,0,0,0);
BBox2	BBox2::BoundBox(WBOUNDS_N,WBOUNDS_N,WBOUNDS_P,WBOUNDS_P);

BBox3	BBox3::NullBox(0,0,0,0,0,0);
BBox3	BBox3::BoundBox(WBOUNDS_N,WBOUNDS_N,WBOUNDS_N,WBOUNDS_P,WBOUNDS_P,WBOUNDS_P);
BBox3	BBox3::InitBox(WBOUNDS_P, WBOUNDS_P, WBOUNDS_P, WBOUNDS_N, WBOUNDS_N, WBOUNDS_N);


BBox2::BBox2() : 
    _min(BoundBox._min),
    _max(BoundBox._max)
{
}

BBox2::BBox2(const Vector2& min, const Vector2& max) :
    _min(min),
    _max(max)
{
}

BBox2::BBox2(const BBox2& b) :
    _min(b._min),
    _max(b._max)
{
}

BBox2::BBox2(float xmin, float ymin, float xmax, float ymax) :
    _min(xmin, ymin),
    _max(xmax,ymax)
{
}

const Vector2&	BBox2::getMin() const
{
    return _min;
}

const Vector2& BBox2::getMax() const
{
    return _max;
}

void BBox2::getMinMax(Vector2& min, Vector2& max)
{
    min=_min;
    max=_max;
}

void BBox2::setMin(const Vector2& min)
{
    _min=min;
}

void BBox2::setMax(const Vector2& max)
{
    _max=max;
}

void BBox2::setMinMax(const Vector2& min, const Vector2& max)
{
    _min=min;
    _max=max;
}

BBox2& BBox2::operator=(const BBox2& b)
{
    _min=b._min;
    _max=b._max;
    return *this;
}

BBox2 BBox2::operator+(const Vector2& v) const
{
    return BBox2(_min+v, _max+v);
}

BBox2 BBox2::operator-(const Vector2& v) const
{
    return BBox2(_min-v, _max-v);
}

BBox2& BBox2::operator+=(const Vector2& v)
{
    _min+=v;
    _max+=v;
    return *this;
}

BBox2& BBox2::operator-=(const Vector2& v)
{
    _min-=v;
    _max-=v;
    return *this;
}

BBox2& BBox2::operator << (const Vector2& v)
{
    _min.x=smin(_min.x,v.x); _min.y=smin(_min.y,v.y);
    _max.x=smax(_max.x,v.x); _max.y=smax(_max.y,v.y);
    return *this;
}

BBox2& BBox2::operator << (const BBox2& b)
{
    return (*this) << b._min << b._max;
}

bool BBox2::isIn(const Vector2& v) const
{
    return (v.x >= _min.x && v.x <= _max.x) && (v.y >= _min.y && v.y <= _max.y);
}

bool BBox2::isIn(const BBox2& b) const
{
    return !(b._max.x < _min.x || b._min.x > _max.x || b._max.y < _min.y || b._min.y > _max.y);
}

bool BBox2::isCompletlyIn(const BBox2& b) const
{
    return b._min.x >= _min.x && b._max.x <= _max.x && b._min.y >= _min.y && b._max.y <= _max.y;
}

void BBox2::repair()
{
    if(_min.x > _max.x)			sswap(_min.x,_max.x);
    if(_min.y > _max.y)			sswap(_min.y,_max.y);
}

Vector2 BBox2::getSize() const
{
    return _max - _min;
}

void BBox2::getSize(Vector2& size) const
{
    size=_max-_min;
}

Vector2 BBox2::getCenter() const
{
    return (_max + _min) * 0.5f;
}

void BBox2::getCenter(Vector2& c) const
{
    c=(_max + _min) * 0.5f;
}



BBox3::BBox3() :
    _min(BoundBox._min),
    _max(BoundBox._max)
{
}

BBox3::BBox3(const Vector3& min, const Vector3& max) :
    _min(min),
    _max(max)
{
}

BBox3::BBox3(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax) :
    _min(xmin,ymin,zmin),
    _max(xmax,ymax,zmax)
{
}

BBox3::BBox3(const BBox3& b) :
    _min(b._min),
    _max(b._max)
{
}

BBox3::BBox3(const BSphere& b) :
    _min(b.getCenter()-b.getRadius()),
    _max(b.getCenter()+b.getRadius())
{
}

void BBox3::createCenterSize(const Vector3& c, const Vector3& s)
{
    _min=c-(s*0.5f);
    _max=c+(s*0.5f);
}

const Vector3& BBox3::getMin() const
{
    return _min;
}

const Vector3&	BBox3::getMax() const
{
    return _max;
}

void BBox3::getMinMax(Vector3& min, Vector3& max) const
{
    min=_min;
    max=_max;
}

void BBox3::setMin(const Vector3& min)
{
    _min=min;
}

void BBox3::setMax(const Vector3& max)
{
    _max=max;
}

void BBox3::setMinMax(const Vector3& min, const Vector3& max)
{
    _min=min;
    _max=max;
}

Vector3 BBox3::getCenter() const
{
    return (_min + _max) * 0.5f;
}

void BBox3::getCenter(Vector3& c) const
{
    c=(_min + _max) * 0.5f;
}

Vector3 BBox3::getSize() const
{
    return (_max-_min);
}

void BBox3::getSize(Vector3& s) const
{
    s=(_max-_min);
}

BBox3& BBox3::operator=(const BBox3& b)
{
    _min=b._min;
    _max=b._max;
    return *this;
}

BBox3& BBox3::operator=(const BSphere& b)
{
    _min=b.getCenter()-b.getRadius();
    _max=b.getCenter()+b.getRadius();
    return *this;
}

BBox3 BBox3::operator+(const Vector3& v) const
{
    return BBox3(_min+v,_max+v);
}

BBox3 BBox3::operator-(const Vector3& v) const
{
    return BBox3(_min-v,_max-v);
}

BBox3& BBox3::operator+=(const Vector3& v)
{
    _min+=v;
    _max+=v;
    return *this;
}

BBox3& BBox3::operator-=(const Vector3& v)
{
    _min-=v;
    _max-=v;
    return *this;
}

BBox3& BBox3::operator << (const Vector3& v)
{
    _min.x=smin(_min.x,v.x); _min.y=smin(_min.y,v.y); _min.z=smin(_min.z,v.z);
    _max.x=smax(_max.x,v.x); _max.y=smax(_max.y,v.y); _max.z=smax(_max.z,v.z);
    return *this;
}

BBox3& BBox3::operator << (const BBox3& b)
{
    return (*this) << b._min << b._max;
}

BBox3& BBox3::operator << (const BSphere& b)
{
    return (*this) << BBox3(b);
}

BBox3 BBox3::operator+(const BBox3& b) const
{
    return BBox3(smin(_min.x,b._min.x),smin(_min.y,b._min.y),smin(_min.z,b._min.z),
        smax(_max.x,b._max.x),smax(_max.y,b._max.y),smax(_max.z,b._max.z));
}

BBox3& BBox3::operator+=(const BBox3& b)
{
    return (*this) << b;
}

BBox3 BBox3::operator*(const Matrix4& m) const
{
    Vector3 center(getCenter() * m);
    Vector3 size(getSize() * 0.5f);
    size.transformNoTranslation(m);
    return BBox3(center - size, center + size);
}

BBox3& BBox3::operator*=(const Matrix4& m)
{
    Vector3 center(getCenter() * m);
    Vector3 size(getSize() * 0.5f);
    size.transformNoTranslation(m);
    _min=center - size;
    _max=center + size;
    return *this;
}

void BBox3::transformFromBox(const BBox3& b, const Matrix4& m)
{
    Vector3 center(b.getCenter() * m);
    Vector3 size(b.getSize() * 0.5f);
    size.transformNoTranslation(m);
    _min=center - size;
    _max=center + size;
    repair();
}

bool BBox3::isValid() const
{
    return _min.x < _max.x && _min.y < _max.y && _min.z < _max.z;
}

void BBox3::repair()
{
    if(_min.x > _max.x)			sswap(_min.x,_max.x);
    if(_min.y > _max.y)			sswap(_min.y,_max.y);
    if(_min.z > _max.z)			sswap(_min.z,_max.z);
}

void BBox3::getSphere(BSphere& b) const
{
    b.setCenter(getCenter());
    b.setRadius(getSize().getLength() * 0.5f);
}

bool BBox3::isIn(const Vector3& v) const
{
    return v.x >= _min.x && v.y >= _min.y && v.z >= _min.z && v.x <= _max.x && v.y <= _max.y && v.z <=_max.z;
}

bool BBox3::isIn(const BBox3& b) const
{
    return (b._max.x >= _min.x && b._min.x <= _max.x) && (b._max.y >= _min.y && b._min.y <= _max.y) && (b._max.z >= _min.z && b._min.z <= _max.z);
}

bool BBox3::isIn(const Vector3& c, float r) const
{
    return c.x+r >= _min.x && c.x-r <= _max.x &&
            c.y+r >= _min.y && c.y-r <= _max.y &&
            c.z+r >= _min.z && c.z-r <= _max.z;
}

bool BBox3::isCompletlyIn(const BBox3& b) const
{
    return (b._min.x >= _min.x && b._min.y >= _min.y && b._min.z >= _min.z && b._max.x <= _max.x && b._max.y <= _max.y && b._max.z <= _max.z);
}

bool BBox3::isCompletlyIn(const Vector3& c, float r) const
{
    return c.x-r >= _min.x && c.x+r <= _max.x &&
            c.y-r >= _min.y && c.y+r <= _max.y &&
            c.z-r >= _min.z && c.z+r <= _max.z;
}

float BBox3::getDistanceFromSq(const Vector3& p) const
{
    Vector3 delta(Vector3::NullVector);

    if(p.x < _min.x) delta.x=_min.x - p.x;
    else if(p.x > _max.x) delta.x=p.x - _max.x;

    if(p.y < _min.y) delta.y=_min.y - p.y;
    else if(p.y > _max.y) delta.y=p.y - _max.y;

    if(p.z < _min.z) delta.z=_min.z - p.z;
    else if(p.z > _max.z) delta.z=p.z - _max.z;

    return delta.getSquareLength();
}

float BBox3::getDistanceFrom(const Vector3& p) const
{
    return sqrtf(getDistanceFromSq(p));
}

void BBox3::getPoints(Vector3 *pv) const
{
    pv[0]=_min;
    pv[1].x=_min.x;			pv[1].y=_min.y;			pv[1].z=_max.z;
    pv[2].x=_min.x;			pv[2].y=_max.y;			pv[2].z=_min.z;
    pv[3].x=_min.x;			pv[3].y=_max.y;			pv[3].z=_max.z;
    pv[4].x=_max.x;			pv[4].y=_min.y;			pv[4].z=_min.z;
    pv[5].x=_max.x;			pv[5].y=_min.y;			pv[5].z=_max.z;
    pv[6].x=_max.x;			pv[6].y=_max.y;			pv[6].z=_min.z;
    pv[7]=_max;
}

bool BBox3::rayIntersect(const Vector3& o, const Vector3& d) const
{
	Vector3 vAbsSegDir,c,s;
	float f1,f2;

	getCenter(c);
	getSize(s);

	const Vector3 vDiff(o-c);

	if(vDiff.x*d.x >= 0.0f && FloatMakePositive(vDiff.x) > s.x) return false;
	if(vDiff.y*d.y >= 0.0f && FloatMakePositive(vDiff.y) > s.y) return false;
	if(vDiff.z*d.z >= 0.0f && FloatMakePositive(vDiff.z) > s.z) return false;

	vAbsSegDir.x=FloatMakePositive(d.x);
	vAbsSegDir.y=FloatMakePositive(d.y);
	vAbsSegDir.z=FloatMakePositive(d.z);

	f1=FloatMakePositive(d.y * vDiff.z - d.z * vDiff.y);
	f2=s.y * vAbsSegDir.z + s.z * vAbsSegDir.y;
	if(f1 > f2) return false;

	f1=FloatMakePositive(d.z * vDiff.x - d.x * vDiff.z);
	f2=s.x * vAbsSegDir.z + s.z * vAbsSegDir.x;
	if(f1 > f2) return false;

	f1=FloatMakePositive(d.x * vDiff.y - d.y * vDiff.x);
	f2=s.x * vAbsSegDir.y + s.y * vAbsSegDir.x;
	if(f1 > f2) return false;

	return true;
}
