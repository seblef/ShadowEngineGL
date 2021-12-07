
#include "Matrix4.h"
#include "Matrix2.h"
#include "Vector4.h"
#include <memory.h>

using namespace Core;

Matrix2 Matrix2::Identity;
Matrix3 Matrix3::Identity;
Matrix4 Matrix4::Identity;



void Matrix2::init()
{
    Identity.createIdentity();
}

Matrix2::Matrix2(const Matrix2& _m)
{
    m[0][0]=_m.m[0][0]; m[0][1]=_m.m[0][1];
    m[1][0]=_m.m[1][0]; m[1][1]=_m.m[1][1];
}

Matrix2::Matrix2(const float *fp)
{
    m[0][0]=fp[0]; m[0][1]=fp[1];
    m[1][0]=fp[2]; m[1][1]=fp[2];
}

Matrix2& Matrix2::operator=(const Matrix2& _m)
{
    m[0][0]=_m.m[0][0]; m[0][1]=_m.m[0][1];
    m[1][0]=_m.m[1][0]; m[1][1]=_m.m[1][1];
    return *this;
}

Matrix2 Matrix2::operator*(const Matrix2& _m) const
{
    Matrix2 temp;
    temp.m[0][0]=m[0][0]*_m.m[0][0] + m[0][1]*_m.m[1][0];
    temp.m[0][1]=m[0][0]*_m.m[0][1] + m[0][1]*_m.m[1][1];
    temp.m[1][0]=m[1][0]*_m.m[0][0] + m[1][1]*_m.m[1][0];
    temp.m[1][1]=m[1][0]*_m.m[0][1] + m[1][1]*_m.m[1][1];
    return temp;
}

Matrix2& Matrix2::operator*=(const Matrix2& _m)
{
    Matrix2 temp;
    temp.m[0][0]=m[0][0]*_m.m[0][0] + m[0][1]*_m.m[1][0];
    temp.m[0][1]=m[0][0]*_m.m[0][1] + m[0][1]*_m.m[1][1];
    temp.m[1][0]=m[1][0]*_m.m[0][0] + m[1][1]*_m.m[1][0];
    temp.m[1][1]=m[1][0]*_m.m[0][1] + m[1][1]*_m.m[1][1];

    return (*this)=temp;
}

void Matrix2::createIdentity()
{
    m[0][0]=1.0f; m[0][1]=0.0f;
    m[1][0]=0.0f; m[1][1]=1.0f;
}

void Matrix2::scale(float f)
{
    m[0][0]=f; m[0][1]=0.0f;
    m[1][0]=0.0f; m[1][1]=f;
}

void Matrix2::scale(float fx, float fy)
{
    m[0][0]=fx; m[0][1]=0.0f;
    m[1][0]=0.0f; m[1][1]=fy;
}

void Matrix2::rotation(float angle)
{
    float c=cosf(angle);
    float s=sinf(angle);

    m[0][0]= c; m[0][1]=s;
    m[1][0]=-s; m[1][1]=c;
}

void Matrix2::rotationScale(float angle, float scale)
{

    float c=cosf(angle) * scale;
    float s=sinf(angle) * scale;

    m[0][0]= c; m[0][1]=s;
    m[1][0]=-s; m[1][1]=c;
}

void Matrix2::rotationScale(float angle, float xscale, float yscale)
{
    float c=cosf(angle) * xscale;
    float s=sinf(angle) * yscale;

    m[0][0]= c; m[0][1]=s;
    m[1][0]=-s; m[1][1]=c;
}

bool Matrix2::inverse(Matrix2& _m) const
{
    float d=(m[0][0] * m[1][1] - m[1][0] * m[0][1]);
    if(d==0.0f) return false;

    d=1.0f / d;

    _m.m[0][0]=d*m[1][1];
    _m.m[1][0]=-d*m[1][0];
    _m.m[0][1]=-d*m[0][1];
    _m.m[1][1]=d*m[0][0];
    return true;
}


void Matrix3::init()
{
    Identity.createIdentity();
}

Matrix3::Matrix3(const Matrix3& _m)
{
    memcpy((void *)m,(void *)_m.m,9*sizeof(float));
}

Matrix3::Matrix3(const float *fp)
{
    memcpy((void *)m,fp,9*sizeof(float));
}

Matrix3& Matrix3::operator=(const Matrix3& _m)
{
    memcpy((void *)m,(void *)_m.m,9*sizeof(float));
    return *this;
}

Matrix3 Matrix3::operator*(const Matrix3& _m) const
{
    Matrix3 temp;
    int i,j;

    for(i=0;i<3;++i)
        for(j=0;j<3;++j)
            temp.m[i][j]=m[i][0] * _m.m[0][j] +
                            m[i][1] * _m.m[1][j] +
                            m[i][2] * _m.m[2][j];

    return temp;
}

Matrix3& Matrix3::operator*=(const Matrix3& _m)
{
    Matrix3 temp;
    for(int i=0;i<3;++i)
        for(int j=0;j<3;++j)
            temp.m[i][j]=m[i][0] * _m.m[0][j] +
                            m[i][1] * _m.m[1][j] +
                            m[i][2] * _m.m[2][j];

    return (*this)=temp;
}

Matrix3& Matrix3::operator*=(float f)
{
    for(int i=0;i<3;++i)
        for(int j=0;j<3;++j)
            m[i][j]*=f;

    return *this;
}
Matrix3& Matrix3::operator+=(const Matrix3& _m)
{
    for(int i=0;i<3;++i)
        for(int j=0;j<3;++j)
            m[i][j]+=_m.m[i][j];

    return *this;
}
Matrix3& Matrix3::operator-=(const Matrix3& _m)
{
    for(int i=0;i<3;++i)
        for(int j=0;j<3;++j)
            m[i][j]-=_m.m[i][j];

    return *this;
}

void Matrix3::createIdentity()
{
    m[0][0]=1.0f; m[0][1]=0.0f; m[0][2]=0.0f;
    m[1][0]=0.0f; m[1][1]=1.0f; m[1][2]=0.0f;
    m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=1.0f;
}

void Matrix3::createScale(float x, float y, float z)
{
    m[0][0]=x   ; m[0][1]=0.0f; m[0][2]=0.0f;
    m[1][0]=0.0f; m[1][1]=y   ; m[1][2]=0.0f;
    m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=z   ;
}
    
void Matrix3::createRotateX(float x)
{
    float c=cosf(x);
    float s=sinf(x);

    m[0][0]=1.0f; m[0][1]=0.0f; m[0][2]=0.0f;
    m[1][0]=0.0f; m[1][1]=c;   m[1][2]=s;
    m[2][0]=0.0f; m[2][1]=-s;  m[2][2]=c;
}

void Matrix3::createRotateY(float y)
{
    float c=cosf(y);
    float s=sinf(y);

    m[0][0]=c;   m[0][1]=0.0f; m[0][2]=-s;
    m[1][0]=0.0f; m[1][1]=1.0f; m[1][2]=0.0f;
    m[2][0]=s;   m[2][1]=0.0f; m[2][2]=c;
}

void Matrix3::createRotateZ(float z)
{
    float c=cosf(z);
    float s=sinf(z);

    m[0][0]=c;   m[0][1]=s;   m[0][2]=0.0f;
    m[1][0]=-s;  m[1][1]=c;   m[1][2]=0.0f;
    m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=1.0f;
}

void Matrix3::createRotateXYZ(float x, float y, float z)
{
    float cr=cosf(x), sr=sinf(z);
    float cp=cosf(y), sp=sinf(y);
    float cy=cosf(z), sy=sinf(z);
    float srsp=sr*sp, crsp=cr*sp;

    m[0][0]=cp*cy; m[0][1]=cp*sy; m[0][2]=-sp;
    m[1][0]=srsp*cy-cr*sy;
    m[1][1]=srsp*sy+cr*cy;
    m[1][2]=sr*cp;

    m[2][0]=crsp*cy+sr*sy;
    m[2][1]=crsp*sy-sr*cy;
    m[2][2]=cr*cp;
}

void Matrix3::transpose(Matrix3& mat) const
{
    mat.m[0][0]=m[0][0]; mat.m[0][1]=m[1][0]; mat.m[0][2]=m[2][0];
    mat.m[1][0]=m[0][1]; mat.m[1][1]=m[1][1]; mat.m[1][2]=m[2][1];
    mat.m[2][0]=m[0][2]; mat.m[2][1]=m[1][2]; mat.m[2][2]=m[2][2];
}



void	Matrix4::init()
{
    Identity.createIdentity();
}

Matrix4::Matrix4(const Matrix4& _m)
{
    memcpy((void *)m,(void *)_m.m,16*sizeof(float));
}

Matrix4::Matrix4(const Matrix3& _m)
{
    for(int i=0;i<3;++i)
        for(int j=0;j<3;++j)
            m[i][j]=_m(i,j);
}

Matrix4::Matrix4(const float *fp)
{
    memcpy((void *)m,fp,16*sizeof(float));
}

Matrix4& Matrix4::operator=(const Matrix4& _m)
{
    memcpy((void *)m,(void *)_m.m,16*sizeof(float));
    return *this;
}

Matrix4& Matrix4::operator=(const Matrix3& _m)
{
    for(int i=0;i<3;++i)
        for(int j=0;j<3;++j)
            m[i][j]=_m(i,j);
    return *this;
}

Matrix4 Matrix4::operator*(const Matrix4& _m) const
{
    Matrix4 temp;
    int i,j;

    for(i=0;i<4;++i)
        for(j=0;j<4;++j)
            temp.m[i][j]=m[i][0] * _m.m[0][j] +
                            m[i][1] * _m.m[1][j] +
                            m[i][2] * _m.m[2][j] +
                            m[i][3] * _m.m[3][j];

    return temp;
}

Matrix4& Matrix4::operator*=(const Matrix4& _m)
{
    Matrix4 temp;
    int i,j;

    for(i=0;i<4;++i)
        for(j=0;j<4;++j)
            temp.m[i][j]=m[i][0] * _m.m[0][j] +
                            m[i][1] * _m.m[1][j] +
                            m[i][2] * _m.m[2][j] +
                            m[i][3] * _m.m[3][j];

    return (*this)=temp;
}

void Matrix4::createIdentity()
{
    m[0][0]=1.0f; m[0][1]=0.0f; m[0][2]=0.0f; m[0][3]=0.0f;
    m[1][0]=0.0f; m[1][1]=1.0f; m[1][2]=0.0f; m[1][3]=0.0f;
    m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=1.0f; m[2][3]=0.0f;
    m[3][0]=0.0f; m[3][1]=0.0f; m[3][2]=0.0f; m[3][3]=1.0f;
}

void Matrix4::createTranslate(float x, float y, float z)
{
    m[0][0]=1.0f; m[0][1]=0.0f; m[0][2]=0.0f; m[0][3]=0.0f;
    m[1][0]=0.0f; m[1][1]=1.0f; m[1][2]=0.0f; m[1][3]=0.0f;
    m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=1.0f; m[2][3]=0.0f;
    m[3][0]=x   ; m[3][1]=y   ; m[3][2]=z   ; m[3][3]=1.0f;
}

void Matrix4::createScale(float x, float y, float z)
{
    m[0][0]=x   ; m[0][1]=0.0f; m[0][2]=0.0f; m[0][3]=0.0f;
    m[1][0]=0.0f; m[1][1]=y   ; m[1][2]=0.0f; m[1][3]=0.0f;
    m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=z   ; m[2][3]=0.0f;
    m[3][0]=0.0f; m[3][1]=0.0f; m[3][2]=0.0f; m[3][3]=1.0f;
}
    
void Matrix4::createRotateX(float x)
{
    float c=cosf(x);
    float s=sinf(x);

    m[0][0]=1.0f; m[0][1]=0.0f; m[0][2]=0.0f; m[0][3]=0.0f;
    m[1][0]=0.0f; m[1][1]=c;   m[1][2]=s;   m[1][3]=0.0f;
    m[2][0]=0.0f; m[2][1]=-s;  m[2][2]=c;   m[2][3]=0.0f;
    m[3][0]=0.0f; m[3][1]=0.0f; m[3][2]=0.0f; m[3][3]=1.0f;
}

void Matrix4::createRotateY(float y)
{
    float c=cosf(y);
    float s=sinf(y);

    m[0][0]=c;   m[0][1]=0.0f; m[0][2]=-s;  m[0][3]=0.0f;
    m[1][0]=0.0f; m[1][1]=1.0f; m[1][2]=0.0f; m[1][3]=0.0f;
    m[2][0]=s;   m[2][1]=0.0f; m[2][2]=c;   m[2][3]=0.0f;
    m[3][0]=0.0f; m[3][1]=0.0f; m[3][2]=0.0f; m[3][3]=1.0f;
}

void Matrix4::createRotateZ(float z)
{
    float c=cosf(z);
    float s=sinf(z);

    m[0][0]=c;   m[0][1]=s;   m[0][2]=0.0f; m[0][3]=0.0f;
    m[1][0]=-s;  m[1][1]=c;   m[1][2]=0.0f; m[1][3]=0.0f;
    m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=1.0f; m[2][3]=0.0f;
    m[3][0]=0.0f; m[3][1]=0.0f; m[3][2]=0.0f; m[3][3]=1.0f;
}

void Matrix4::createRotateXYZ(float x, float y, float z)
{
    float cr=cosf(x), sr=sinf(z);
    float cp=cosf(y), sp=sinf(y);
    float cy=cosf(z), sy=sinf(z);
    float srsp=sr*sp, crsp=cr*sp;

    m[0][0]=cp*cy; m[0][1]=cp*sy; m[0][2]=-sp; m[0][3]=0.0f;
    m[1][0]=srsp*cy-cr*sy;
    m[1][1]=srsp*sy+cr*cy;
    m[1][2]=sr*cp; m[1][3]=0.0f;

    m[2][0]=crsp*cy+sr*sy;
    m[2][1]=crsp*sy-sr*cy;
    m[2][2]=cr*cp; m[2][3]=0.0f;

    m[3][0]=m[3][1]=m[3][2]=0.0f; m[3][3]=1.0f;
}

void Matrix4::transpose(Matrix4& mat) const
{
    mat.m[0][0]=m[0][0]; mat.m[0][1]=m[1][0]; mat.m[0][2]=m[2][0]; mat.m[0][3]=m[3][0];
    mat.m[1][0]=m[0][1]; mat.m[1][1]=m[1][1]; mat.m[1][2]=m[2][1]; mat.m[1][3]=m[3][1];
    mat.m[2][0]=m[0][2]; mat.m[2][1]=m[1][2]; mat.m[2][2]=m[2][2]; mat.m[2][3]=m[3][2];
    mat.m[3][0]=m[0][3]; mat.m[3][1]=m[1][3]; mat.m[3][2]=m[2][3]; mat.m[3][3]=m[3][3];
}

void Matrix4::getMatrix3(Matrix3& _m) const
{
    for(int i=0;i<3;++i)
        for(int j=0;j<3;++j)
            _m(i,j)=m[i][j];
}

Matrix4::operator Matrix3() const
{
    Matrix3 r;
    getMatrix3(r);
    return r;
}

void Matrix4::reinitTranslation()
{
    m[3][0]=m[3][1]=m[3][2]=0;
}

void Matrix4::reinitRotationScale()
{
    m[0][0]=1.0f; m[0][1]=0.0f; m[0][2]=0.0f;
    m[1][0]=0.0f; m[1][1]=1.0f; m[1][2]=0.0f;
    m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=1.0f;
}

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
