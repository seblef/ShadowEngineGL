
#include "Camera.h"

using namespace Core;

Camera::Camera() : _pos(Vector3::NullVector), _target(Vector3::ZAxisVector), _center(Vector2::NullVector), _up(Vector3::YAxisVector),
    _ZMin(1.0f), _ZMax(1000.0f), _fov(45.0f), _type(CT_PERSPECTIVE), _zoom(1.0f), _isLH(true)

{
}

Camera::Camera(
	const Vector3& vPos,
	const Vector3& vTarget,
	float fZMin, float fZMax,
	const Vector2& vCenter,
	float fFOV,
	bool isLH
) :
    _pos(vPos),
	_target(vTarget),
	_center(vCenter),
	_up(Vector3::YAxisVector),
	_ZMin(fZMin), _ZMax(fZMax),
	_fov(fFOV),
	_isLH(isLH),
	_type(CT_PERSPECTIVE),
	_zoom(1.0f)
{
}


void Camera::center(const BBox3& bBox)
{
	Vector3 vSize(bBox.getSize());
	Vector3 vCenter(bBox.getCenter());

	Vector3 vDir(_target - _pos);
	vDir.normalize();

	_target=vCenter;
	_pos=vCenter - vDir * vSize.getLength() * 2.0f;
}

void Camera::buildMatrices()
{
	if(_isLH)
	    _viewMatrix.lookAtLH(_pos,_target,_up);
	else
    	_viewMatrix.lookAtRH(_pos,_target,_up);
	_viewMatrix.inverse(_invViewMatrix);

	if(_type==CT_PERSPECTIVE)
	{
		if(_isLH)
        	_projMatrix.perspectiveFOVLH(_fov*M_PI/180.0f,_center.x / _center.y,_ZMin,_ZMax);
		else
        	_projMatrix.perspectiveFOVRH(_fov*M_PI/180.0f,_center.x / _center.y,_ZMin,_ZMax);
	}
	else
	{
		float h=2.0f * _center.y / _zoom;
		float w=2.0f * _center.x / _zoom;
        _projMatrix.orthoLH(w,h,_ZMin,_ZMax);
	}

	_viewProjMatrix=_viewMatrix * _projMatrix;

	_XAxis=Vector3(_viewMatrix(0,0),_viewMatrix(1,0),_viewMatrix(2,0));
	_YAxis=Vector3(_viewMatrix(0,1),_viewMatrix(1,1),_viewMatrix(2,1));
	_XAxis.normalize();
	_YAxis.normalize();
}

void Camera::makeRayFrom2D(
    int x, int y,
    Vector3 &origin,
    Vector3 &dir
)
{
	if(_type == CT_PERSPECTIVE)
	{
		float w = _center.x * 2.0f;
		float h = _center.y * 2.0f;

		Vector3 v;
		v.x = ((( 2.0f * ((float)x)) / w) - 1.0f) / _projMatrix(0,0);
		v.y = -((( 2.0f * ((float)y)) / h) - 1.f) / _projMatrix(1,1);
		v.z = 1.0f;

		dir = v;
		dir.transformNoTranslation(_invViewMatrix);
		dir.normalize();
		origin = _invViewMatrix;
	}
	else
	{
		dir = _target-_pos;
		dir.normalize();
		origin = _pos;
		origin += _XAxis * ((float)x - _center.x) / _zoom;
		origin -= _YAxis * ((float)y - _center.y) / _zoom;
	}
}
