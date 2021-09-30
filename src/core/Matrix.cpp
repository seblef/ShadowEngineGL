
#include "Matrix4.h"
#include "Matrix2.h"
#include "Vector4.h"

using namespace Core;

Matrix2 Matrix2::Identity;
Matrix3 Matrix3::Identity;
Matrix4 Matrix4::Identity;


bool Matrix4::inverse(Matrix4& mat) const
{
	float d=(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) - (m[0][0] * m[2][1] - m[2][0] * m[0][1]) * (m[1][2] * m[3][3] - m[3][2] * m[1][3]) +
			(m[0][0] * m[3][1] - m[3][0] * m[0][1]) * (m[1][2] * m[2][3] - m[2][2] * m[1][3]) + (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * (m[0][2] * m[3][3] - m[3][2] * m[0][3]) -
			(m[1][0] * m[3][1] - m[3][0] * m[1][1]) * (m[0][2] * m[2][3] - m[2][2] * m[0][3]) + (m[2][0] * m[3][1] - m[3][0] * m[2][1]) * (m[0][2] * m[1][3] - m[1][2] * m[0][3]);

	if(d==0.0f)
		return false;

	d=1.0f / d;

	mat.m[0][0]=d * (m[1][1] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) + m[2][1] * (m[3][2] * m[1][3] - m[1][2] * m[3][3]) + m[3][1] * (m[1][2] * m[2][3] - m[2][2] * m[1][3]));
	mat.m[1][0]=d * (m[1][2] * (m[2][0] * m[3][3] - m[3][0] * m[2][3]) + m[2][2] * (m[3][0] * m[1][3] - m[1][0] * m[3][3]) + m[3][2] * (m[1][0] * m[2][3] - m[2][0] * m[1][3]));
	mat.m[2][0]=d * (m[1][3] * (m[2][0] * m[3][1] - m[3][0] * m[2][1]) + m[2][3] * (m[3][0] * m[1][1] - m[1][0] * m[3][1]) + m[3][3] * (m[1][0] * m[2][1] - m[2][0] * m[1][1]));
	mat.m[3][0]=d * (m[1][0] * (m[3][1] * m[2][2] - m[2][1] * m[3][2]) + m[2][0] * (m[1][1] * m[3][2] - m[3][1] * m[1][2]) + m[3][0] * (m[2][1] * m[1][2] - m[1][1] * m[2][2]));

	mat.m[0][1]=d * (m[2][1] * (m[0][2] * m[3][3] - m[3][2] * m[0][3]) + m[3][1] * (m[2][2] * m[0][3] - m[0][2] * m[2][3]) + m[0][1] * (m[3][2] * m[2][3] - m[2][2] * m[3][3]));
	mat.m[1][1]=d * (m[2][2] * (m[0][0] * m[3][3] - m[3][0] * m[0][3]) + m[3][2] * (m[2][0] * m[0][3] - m[0][0] * m[2][3]) + m[0][2] * (m[3][0] * m[2][3] - m[2][0] * m[3][3]));
	mat.m[2][1]=d * (m[2][3] * (m[0][0] * m[3][1] - m[3][0] * m[0][1]) + m[3][3] * (m[2][0] * m[0][1] - m[0][0] * m[2][1]) + m[0][3] * (m[3][0] * m[2][1] - m[2][0] * m[3][1]));
	mat.m[3][1]=d * (m[2][0] * (m[3][1] * m[0][2] - m[0][1] * m[3][2]) + m[3][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]) + m[0][0] * (m[2][1] * m[3][2] - m[3][1] * m[2][2]));

	mat.m[0][2]=d * (m[3][1] * (m[0][2] * m[1][3] - m[1][2] * m[0][3]) + m[0][1] * (m[1][2] * m[3][3] - m[3][2] * m[1][3]) + m[1][1] * (m[3][2] * m[0][3] - m[0][2] * m[3][3]));
	mat.m[1][2]=d * (m[3][2] * (m[0][0] * m[1][3] - m[1][0] * m[0][3]) + m[0][2] * (m[1][0] * m[3][3] - m[3][0] * m[1][3]) + m[1][2] * (m[3][0] * m[0][3] - m[0][0] * m[3][3]));
	mat.m[2][2]=d * (m[3][3] * (m[0][0] * m[1][1] - m[1][0] * m[0][1]) + m[0][3] * (m[1][0] * m[3][1] - m[3][0] * m[1][1]) + m[1][3] * (m[3][0] * m[0][1] - m[0][0] * m[3][1]));
	mat.m[3][2]=d * (m[3][0] * (m[1][1] * m[0][2] - m[0][1] * m[1][2]) + m[0][0] * (m[3][1] * m[1][2] - m[1][1] * m[3][2]) + m[1][0] * (m[0][1] * m[3][2] - m[3][1] * m[0][2]));

	mat.m[0][3]=d * (m[0][1] * (m[2][2] * m[1][3] - m[1][2] * m[2][3]) + m[1][1] * (m[0][2] * m[2][3] - m[2][2] * m[0][3]) + m[2][1] * (m[1][2] * m[0][3] - m[0][2] * m[1][3]));
	mat.m[1][3]=d * (m[0][2] * (m[2][0] * m[1][3] - m[1][0] * m[2][3]) + m[1][2] * (m[0][0] * m[2][3] - m[2][0] * m[0][3]) + m[2][2] * (m[1][0] * m[0][3] - m[0][0] * m[1][3]));
	mat.m[2][3]=d * (m[0][3] * (m[2][0] * m[1][1] - m[1][0] * m[2][1]) + m[1][3] * (m[0][0] * m[2][1] - m[2][0] * m[0][1]) + m[2][3] * (m[1][0] * m[0][1] - m[0][0] * m[1][1]));
	mat.m[3][3]=d * (m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) + m[1][0] * (m[2][1] * m[0][2] - m[0][1] * m[2][2]) + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

	return true;
}

void Matrix4::lookAtLH(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	Vector3 z(target - pos);
	z.normalize();

	Vector3 x(up ^ z);
	x.normalize();

	Vector3 y(z ^ x);

	float dx=-(x * pos);
	float dy=-(y * pos);
	float dz=-(z * pos);

	m[0][0]=x.x;	m[0][1]=y.x;	m[0][2]=z.x;	m[0][3]=0.0f;
	m[1][0]=x.y;	m[1][1]=y.y;	m[1][2]=z.y;	m[1][3]=0.0f;
	m[2][0]=x.z;	m[2][1]=y.z;	m[2][2]=z.z;	m[2][3]=0.0f;
	m[3][0]=dx;		m[3][1]=dy;		m[3][2]=dz;		m[3][3]=1.0f;
}

void Matrix4::lookAtRH(const Vector3& pos, const Vector3& target, const Vector3& up)
{
    Vector3 z(pos - target);
    z.normalize();

    Vector3 x(up ^ z);
    x.normalize();

    Vector3 y(z ^ x);

    float dx=-(x * pos);
    float dy=-(y * pos);
    float dz=-(z * pos);

    m[0][0]=x.x;	m[0][1]=y.x;	m[0][2]=z.x;	m[0][3]=0.0f;
    m[1][0]=x.y;	m[1][1]=y.y;	m[1][2]=z.y;	m[1][3]=0.0f;
    m[2][0]=x.z;	m[2][1]=y.z;	m[2][2]=z.z;	m[2][3]=0.0f;
    m[3][0]=dx;		m[3][1]=dy;		m[3][2]=dz;		m[3][3]=1.0f;
}

void Matrix4::orthoLH(float width, float height, float near, float far)
{
	createIdentity();
	float invz=1.0f / (far - near);

	m[0][0]=2.0f / width;
	m[1][1]=2.0f / height;
	m[2][2]=invz;
	m[3][2]=near * invz;
}

void Matrix4::perspectiveFOVLH(float fov, float aspect, float near, float far)
{
	createIdentity();
	float h=1.0f / tanf(fov * 0.5f);
	float w=h / aspect;
	float invz=far / (far - near);

	m[0][0]=w;
	m[1][1]=h;
	m[2][2]=invz;
	m[3][2]=-near * invz;
	m[2][3]=1.0f;
	m[3][3]=0.0f;
}

void Matrix4::perspectiveFOVRH(float fov, float aspect, float near, float far)
{
    createIdentity();
    float h=1.0f / tanf(fov * 0.5f);
    float w=h / aspect;
    float invz=far / (near - far);

    m[0][0]=w;
    m[1][1]=h;
    m[2][2]=invz;
    m[3][2]=near * invz;
    m[2][3]=-1.0f;
    m[3][3]=0.0f;
}

Matrix4& Matrix4::operator=(const Vector3& v)
{
	m[3][0]=v.x;
	m[3][1]=v.y;
	m[3][2]=v.z;

	return *this;
}

Matrix4& Matrix4::operator=(const Vector4& v)
{
	m[3][0]=v.x;
	m[3][1]=v.y;
	m[3][2]=v.z;
	m[3][3]=v.w;

	return *this;
}
