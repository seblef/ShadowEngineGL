
#include "Quaternion.h"
#include "MathUtils.h"
#include "CoreCommon.h"


using namespace Core;


void Quaternion::normalize()
{
    float fNorm=x*x + y*y + z*z + w*w;
    if(fNorm < SEPSILON)
    {
        x=y=z=0.0f;
        w=1.0f; 
    }
    else if(FloatMakePositive(fNorm - 1.0f) > SEPSILON)
    {
        const float fScale=FastInvSqrt(fNorm);
        x*=fScale; y*=fScale; z*=fScale; w*=fScale;
    }
}

Vector3 Quaternion::operator*(const Vector3& v) const
{
    Vector3 v0(x,y,z);
    Vector3 v1(v0 ^ v);
    Vector3 v2(v0 ^ v1);
    v1*=(2.0f*w);
    v2*=2.0f;

    return v+v1+v2;
}

void Quaternion::buildMatrix(const Matrix4& mat)
{
	float fTrace=mat(0,0) + mat(1,1) + mat(2,2);
	float fRoot;
	if(fTrace > 0.0f)
	{
		fRoot=sqrtf(fTrace + 1.0f);
		w=0.5f*fRoot;
		fRoot=0.5f / fRoot;
		x=(mat(2,1) - mat(1,2))*fRoot;
		y=(mat(0,2) - mat(2,0))*fRoot;
		z=(mat(1,0) - mat(0,1))*fRoot;
	}
	else
	{
		int iNext[3]={1,2,0};
		int i=0;
		if(mat(1,1) > mat(0,0))
			i=1;

		if(mat(2,2) > mat(i,i))
			i=2;

		int j=iNext[i]; 
		int k=iNext[j];
		fRoot=sqrtf(mat(i,i) - mat(j,j) - mat(k,k) + 1.0f);
		float *pQuat[3]={&x,&y,&z};
		*pQuat[i]=0.5f*fRoot;

		fRoot=0.5f/fRoot;
		w=(mat(k,j) - mat(j,k))*fRoot;
		*(pQuat[j])=(mat(j,i) + mat(i,j))*fRoot;
		*(pQuat[k])=(mat(k,i) + mat(i,k))*fRoot;
	}

	normalize();
}

void Quaternion::buildMatrix(const Matrix3& mat)
{
	float fTrace=mat(0,0) + mat(1,1) + mat(2,2);
	float fRoot;
	if(fTrace > 0.0f)
	{
		fRoot=sqrtf(fTrace + 1.0f);
		w=0.5f*fRoot;
		fRoot=0.5f / fRoot;
		x=(mat(2,1) - mat(1,2))*fRoot;
		y=(mat(0,2) - mat(2,0))*fRoot;
		z=(mat(1,0) - mat(0,1))*fRoot;
	}
	else
	{
		int iNext[3]={1,2,0};
		int i=0;
		if(mat(1,1) > mat(0,0))
			i=1;

		if(mat(2,2) > mat(i,i))
			i=2;

		int j=iNext[i]; 
		int k=iNext[j];
		fRoot=sqrtf(mat(i,i) - mat(j,j) - mat(k,k) + 1.0f);
		float *pQuat[3]={&x,&y,&z};
		*pQuat[i]=0.5f*fRoot;

		fRoot=0.5f/fRoot;
		w=(mat(k,j) - mat(j,k))*fRoot;
		*(pQuat[j])=(mat(j,i) + mat(i,j))*fRoot;
		*(pQuat[k])=(mat(k,i) + mat(i,k))*fRoot;
	}

	normalize();
}

void Quaternion::buildAxisAngle(const Vector3& vAxis, float fAngle)
{
	float fHalfAngle=fAngle*0.5f;
	float fSin=sinf(fHalfAngle);
	x=fSin*vAxis.x;
	y=fSin*vAxis.y;
	z=fSin*vAxis.z;
	w=cosf(fHalfAngle);

	normalize();
}

void Quaternion::buildEuler(float ex, float ey, float ez)
{
	float fAngle,sr,sp,sy,cr,cp,cy;
	fAngle=ez*0.5f; sy=sinf(fAngle); cy=cosf(fAngle);
	fAngle=ey*0.5f; sp=sinf(fAngle); cp=cosf(fAngle);
	fAngle=ex*0.5f; sr=sinf(fAngle); cr=cosf(fAngle);

	float cpcy=cp*cy;
	float spcy=sp*cy;
	float cpsy=cp*sy;
	float spsy=sp*sy;

	x=sr * cpcy - cr * spsy;
	y=cr * spcy + sr * cpsy;
	z=cr * cpsy - sr * spcy;
	w=cr * cpcy + sr * spsy;

	Quaternion qX(Vector3::XAxisVector,ex);
	Quaternion qY(Vector3::YAxisVector,ey);
	Quaternion qZ(Vector3::ZAxisVector,ez);

	mult(qY,qX,*this);
	mult(qZ,*this,*this);

	normalize();
}

void Quaternion::buildRotationArc(const Vector3& vFrom, const Vector3& vTo)
{
	Vector3 v0(vFrom);
	Vector3 v1(vTo);

	v0.normalize();
	v1.normalize();

	Vector3 c(v0^v1);
	float d=v0*v1;

	if(d>=1.0f)
		reset();
	else
	{
		float invs=FastInvSqrt((1.0f + d)*2.0f);
		float s=1.0f / invs;

		x=c.x * invs;
		y=c.y * invs;
		z=c.z * invs;
		w=s*0.5f;
	}
}

void Quaternion::getMatrix(Matrix4& mat) const
{
	float fTX=2.0f*x, fTY=2.0f*y, fTZ=2.0f*z;
	float fTWX=fTX*w, fTWY=fTY*w, fTWZ=fTZ*w;
	float fTXX=fTX*x, fTXY=fTY*x, fTXZ=fTZ*x;
	float fTYY=fTY*y, fTYZ=fTZ*y;
	float fTZZ=fTZ*z;
	mat(0,0)=1.0f - (fTYY+fTZZ); mat(0,1)=fTXY - fTWZ;		mat(0,2)=fTXZ + fTWY;		mat(0,3)=0.0f;
	mat(1,0)=fTXY + fTWZ;		mat(1,1)=1.0f - (fTXX+fTZZ); mat(1,2)=fTYZ - fTWX;		mat(1,3)=0.0f;
	mat(2,0)=fTXZ - fTWY;		mat(2,1)=fTYZ + fTWX;		mat(2,2)=1.0f - (fTXX+fTYY); mat(2,3)=0.0f;
	mat(3,0)=0.0f;				mat(3,1)=0.0f;				mat(3,2)=0.0f;				mat(3,3)=1.0f;
}

void Quaternion::getMatrix(Matrix3& mat) const
{
	float fTX=2.0f*x, fTY=2.0f*y, fTZ=2.0f*z;
	float fTWX=fTX*w, fTWY=fTY*w, fTWZ=fTZ*w;
	float fTXX=fTX*x, fTXY=fTY*x, fTXZ=fTZ*x;
	float fTYY=fTY*y, fTYZ=fTZ*y;
	float fTZZ=fTZ*z;
	mat(0,0)=1.0f - (fTYY+fTZZ); mat(0,1)=fTXY - fTWZ;		mat(0,2)=fTXZ + fTWY;
	mat(1,0)=fTXY + fTWZ;		mat(1,1)=1.0f - (fTXX+fTZZ); mat(1,2)=fTYZ - fTWX;
	mat(2,0)=fTXZ - fTWY;		mat(2,1)=fTYZ + fTWX;		mat(2,2)=1.0f - (fTXX+fTYY);
}

void Quaternion::getAxisAngle(Vector3& vAxis, float &fAngle) const
{
	float fSqrLen=x*x+y*y+z*z;
	if(fSqrLen > 0.0f)
	{
		fAngle=2.0f * acosf(w);
		float fInvLen=1.0f / logf(fSqrLen);
		vAxis.x=x*fInvLen; vAxis.y=y*fInvLen; vAxis.z=z*fInvLen;
	}
	else
	{
		fAngle=0.0f;
		vAxis.x=1.0f; vAxis.y=0.0f; vAxis.z=0.0f;
	}
}

void Quaternion::align(const Quaternion& q1, const Quaternion& q2, Quaternion& qRet)
{
    float ta=q1.x- q2.x;
    float tb=q1.x+ q2.x;

    float a=ta*ta;
    float b=tb*tb;

    ta=q1.y-q2.y; tb=q1.y+q2.y;
    a+=ta*ta; b+=tb*tb;

    ta=q1.z-q2.z; tb=q1.z+q2.z;
    a+=ta*ta; b+=tb*tb;

    ta=q1.w-q2.w; tb=q1.w+q2.w;
    a+=ta*ta; b+=tb*tb;

    if(a > b)
    {
        qRet.x=-q2.x;
        qRet.y=-q2.y;
        qRet.z=-q2.z;
        qRet.w=-q2.w;
    }
    else if(&qRet!=&q1)
        qRet=q2;
}

void Quaternion::mult(const Quaternion& q1, const Quaternion& q2, Quaternion& qRet)
{
    Quaternion qTemp;
    align(q1,q2,qTemp);

    qRet.x= q1.x * qTemp.w + q1.y * qTemp.z - q1.z * qTemp.y + q1.w * qTemp.x;
    qRet.y=-q1.x * qTemp.z + q1.y * qTemp.w + q1.z * qTemp.x + q1.w * qTemp.y;
    qRet.z= q1.x * qTemp.y - q1.y * qTemp.x + q1.z * qTemp.w + q1.w * qTemp.z;
    qRet.w=-q1.x * qTemp.x - q1.y * qTemp.y - q1.z * qTemp.z + q1.w * qTemp.w;
}

void Quaternion::slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& qRet)
{
    Quaternion qTemp;
    align(q1,q2,qTemp);

    float fSclQ1, fSclQ2;

    const float fCos=q1.x * qTemp.x + q1.y * qTemp.y + q1.z * qTemp.z + q1.w * qTemp.w;

    if((1.0f + fCos) > 0.0000001f)
    {
        if((1.0f - fCos) > 0.000001f)
        {
            const float fOmega=acosf(fCos);
            const float fSin=1.0f / sinf(fOmega);
            fSclQ1=sinf((1.0f - t) * fOmega) * fSin;
            fSclQ2=sinf(t*fOmega) * fSin;
        }
        else
        {
            fSclQ1=1.0f -t;
            fSclQ2=t;
        }

        qRet.x=fSclQ1 * q1.x + fSclQ2 * qTemp.x;
        qRet.y=fSclQ1 * q1.y + fSclQ2 * qTemp.y;
        qRet.z=fSclQ1 * q1.z + fSclQ2 * qTemp.z;
        qRet.w=fSclQ1 * q1.w + fSclQ2 * qTemp.w;
    }
    else
    {
        qRet.x=-q1.y;
        qRet.y= q1.x;
        qRet.z=-q1.w;
        qRet.w= q1.z;

        fSclQ1=sinf((1.0f - t) * (0.5f * M_PI));
        fSclQ2=sinf(t * (0.5f * M_PI));

        qRet.x=fSclQ1 * q1.x + fSclQ2 * qTemp.x;
        qRet.y=fSclQ1 * q1.y + fSclQ2 * qTemp.y;
        qRet.z=fSclQ1 * q1.z + fSclQ2 * qTemp.z;
        qRet.w=fSclQ1 * q1.w + fSclQ2 * qTemp.w;
    }

    qRet.normalize();
}
