
#pragma once

#include "Matrix3.h"

namespace Core
{

class Vector3;
class Vector4;

class Matrix4
{
protected:
	
	float			m[4][4];

public:

	static Matrix4	Identity;

	static void		init();

	Matrix4()		{}
	~Matrix4()		{}

	Matrix4(const Matrix4& _m);
	Matrix4(const Matrix3& _m);
	Matrix4(const float *fp);

	float			operator()(int i, int j) const				{ return m[i][j]; }
	float&			operator()(int i, int j)					{ return m[i][j]; }

	Matrix4&		operator=(const Matrix4& _m);
	Matrix4&		operator=(const Matrix3& _m);

	Matrix4			operator*(const Matrix4& _m) const;
	Matrix4&		operator*=(const Matrix4& _m);

	void			createIdentity();
	void			createTranslate(float x, float y, float z);
	void			createScale(float x, float y, float z);
	void			createRotateX(float x);
	void			createRotateY(float y);
	void			createRotateZ(float z);
	void			createRotateXYZ(float x, float y, float z);

	bool			inverse(Matrix4& mat) const;
	void			transpose(Matrix4& mat) const;

	void			lookAtLH(const Vector3& pos, const Vector3& target, const Vector3& up);
    void            lookAtRH(const Vector3& pos, const Vector3& target, const Vector3& up);
	void			orthoLH(float width, float height, float near, float far);
	void			perspectiveFOVLH(float fov, float aspect, float near, float far);
    void            perspectiveFOVRH(float fov, float aspace, float near, float far);

	void			getMatrix3(Matrix3& _m) const;
					operator Matrix3() const;

	void			reinitTranslation();
	void			reinitRotationScale();

	Matrix4&	operator=(const Vector3& v);
	Matrix4&	operator=(const Vector4& v);
};

inline ostream& operator << (ostream& os, const Matrix4& m)
{
	for(int i=0;i<4;++i)
		os << "| " << m(i,0) << " , " << m(i,1) << " , " << m(i,2) << " , " << m(i,3) << " |" << endl;

	return os;
}

}
