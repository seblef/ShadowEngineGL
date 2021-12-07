
#include "ViewFrustum.h"

using namespace Core;

ViewFrustum::ViewFrustum() :
    _builtFromMatrix(false),
    _planeCount(0),
    _viewProj(Matrix4::Identity)
{
}

ViewFrustum::ViewFrustum(const Matrix4& mat)
{
    createFromViewProjMatrix(mat);
}

ViewFrustum::ViewFrustum(const ViewFrustum& vf)
{
    *this=vf;
}

ViewFrustum& ViewFrustum::operator=(const ViewFrustum& vf)
{
    _planeCount=vf._planeCount;
    _position=vf._position;
    _builtFromMatrix=vf._builtFromMatrix;
    _viewProj=vf._viewProj;

    for(int p=0;p<_planeCount;++p)
        _planes[p]=vf._planes[p];

    return *this;
}

void ViewFrustum::reinit()
{
    _planeCount=0;
    _builtFromMatrix=false;
    _position=Vector3::NullVector;
    _viewProj.createIdentity();
}

void ViewFrustum::setPosition(const Vector3& vPos)
{
    _position=vPos;
}

const Vector3& ViewFrustum::getPosition() const
{
    return _position;
}

void ViewFrustum::transform(const Matrix4& mat)
{
    for(int p=0;p<_planeCount;++p)
        _planes[p]*=mat;
    _position*=mat;
}

const Matrix4& ViewFrustum::getViewProjMatrix() const
{
    return _viewProj;
}

void ViewFrustum::addPlane(const Plane& p)
{
    assert(_planeCount < FRUSTRUM_MAX_PLANES);
    _planes[_planeCount]=p;
    ++_planeCount;
    _builtFromMatrix=false;
}

void ViewFrustum::addPlane(const Vector3& v1, const Vector3& v2)
{
    addPlane(Plane(_position,v1,v2));
}

int ViewFrustum::getPlaneCount() const
{
    return _planeCount;
}

const Plane& ViewFrustum::getPlane(int n) const
{
    return _planes[n];
}

bool ViewFrustum::isPointInside(const Vector3& v) const
{
    for(int p=0;p<_planeCount;++p)
        if(_planes[p].classifyPoint(v)==PLANE_FRONT)
            return false;

    return true;
}

bool ViewFrustum::isLineInside(const Vector3& start, const Vector3& end) const
{
    for(int p=0;p<_planeCount;++p)
        if(_planes[p].classifyPoint(start)==PLANE_FRONT &&
            _planes[p].classifyPoint(end)==PLANE_FRONT)
            return false;

    return true;
}

bool ViewFrustum::isBoxInside(const BBox3& v) const
{
    for(int p=0;p<_planeCount;++p)
        if(_planes[p].classifyBox(v)==PLANE_FRONT)
            return false;

    return true;
}

bool ViewFrustum::isSphereInside(const BSphere& s) const
{
    for(int p=0;p<_planeCount;++p)
        if(_planes[p].classifySphere(s)==PLANE_FRONT)
            return false;

    return true;
}

bool ViewFrustum::isMatrixCreated() const
{
    return _builtFromMatrix;
}

Vector3 ViewFrustum::getFarLeftUp() const
{
    Vector3 p;	_planes[VFP_FAR].intersectWithPlanes(_planes[VFP_TOP],_planes[VFP_LEFT],p); return p;
}

Vector3 ViewFrustum::getFarLeftDown() const
{
    Vector3 p;	_planes[VFP_FAR].intersectWithPlanes(_planes[VFP_BOTTOM],_planes[VFP_LEFT],p); return p;
}

Vector3 ViewFrustum::getFarRightUp() const
{
    Vector3 p;	_planes[VFP_FAR].intersectWithPlanes(_planes[VFP_TOP],_planes[VFP_RIGHT],p); return p;
}

Vector3 ViewFrustum::getFarRightDown() const
{
    Vector3 p;	_planes[VFP_FAR].intersectWithPlanes(_planes[VFP_BOTTOM],_planes[VFP_RIGHT],p); return p;
}

void ViewFrustum::createFromViewProjMatrix(const Matrix4& mat, bool bNearAndFar)
{

#define sw(a,b) (mat(a,b))

	float len;
	Vector3 norm;
	norm.x=-(sw(0,3) + sw(0,0));
	norm.y=-(sw(1,3) + sw(1,0));
	norm.z=-(sw(2,3) + sw(2,0));
	len=1.0f / norm.getLength();
	norm*=len;
	_planes[VFP_LEFT].makePlane(norm,-(sw(3,3) + sw(3,0)) * len);

	norm.x=-(sw(0,3) - sw(0,0));
	norm.y=-(sw(1,3) - sw(1,0));
	norm.z=-(sw(2,3) - sw(2,0));
	len=1.0f / norm.getLength();
	norm*=len;
	_planes[VFP_RIGHT].makePlane(norm,-(sw(3,3) - sw(3,0)) * len);

	norm.x=-(sw(0,3) - sw(0,1));
	norm.y=-(sw(1,3) - sw(1,1));
	norm.z=-(sw(2,3) - sw(2,1));
	len=1.0f / norm.getLength();
	norm*=len;
	_planes[VFP_TOP].makePlane(norm,-(sw(3,3) - sw(3,1)) * len);

	norm.x=-(sw(0,3) + sw(0,1));
	norm.y=-(sw(1,3) + sw(1,1));
	norm.z=-(sw(2,3) + sw(2,1));
	len=1.0f / norm.getLength();
	norm*=len;
	_planes[VFP_BOTTOM].makePlane(norm,-(sw(3,3) + sw(3,1)) * len);
	 
	if(bNearAndFar)
	{
		norm.x=-sw(0,2);
		norm.y=-sw(1,2);
		norm.z=-sw(2,2);
		len=1.0f / norm.getLength();
		norm*=len;
		_planes[VFP_NEAR].makePlane(norm,-sw(3,2) * len);

		norm.x=-(sw(0,3) - sw(0,2));
		norm.y=-(sw(1,3) - sw(1,2));
		norm.z=-(sw(2,3) - sw(2,2));
		len=1.0f / norm.getLength();
		norm*=len;
		_planes[VFP_FAR].makePlane(norm,-(sw(3,3) - sw(3,2)) * len);

		_planeCount=6;
	}
	else
		_planeCount=4;

	_builtFromMatrix=true;
	_viewProj=mat;
}
