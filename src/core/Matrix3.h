
#pragma once

#include "Matrix2.h"

namespace Core
{

class Matrix3
{
protected:
	
	float			m[3][3];

public:

	static Matrix3	Identity;

    static void init();

	Matrix3()		{}
	~Matrix3()		{}

	Matrix3(const Matrix3& _m);
	Matrix3(const float *fp);

	Matrix3&		operator=(const Matrix3& _m);

	Matrix3			operator*(const Matrix3& _m) const;
	Matrix3&		operator*=(const Matrix3& _m);
	Matrix3&		operator*=(float f);

	Matrix3&		operator+=(const Matrix3& _m);
	Matrix3&		operator-=(const Matrix3& _m);

	float			operator()(int i, int j) const				{ return m[i][j]; }
	float&			operator()(int i, int j)					{ return m[i][j]; }

	void			createIdentity();
	void			createScale(float x, float y, float z);
	void			createRotateX(float x);
	void			createRotateY(float y);
	void			createRotateZ(float z);
	void			createRotateXYZ(float x, float y, float z);

	void			transpose(Matrix3& mat) const;
};

inline ostream& operator << (ostream& os, const Matrix3& m)
{
	for(int i=0;i<3;++i)
		os << "| " << m(i,0) << " , " << m(i,1) << " , " << m(i,2) << " |" << endl;

	return os;
}

}
