
#pragma once

#include "MathUtils.h"
#include <ostream>

using namespace std;

namespace Core
{

class Matrix2
{
protected:

	float			m[2][2];

public:

	static Matrix2	Identity;

	static void		init();

	Matrix2() {}
	Matrix2(const Matrix2& _m);
	Matrix2(const float *fp);
	~Matrix2() {}

	float			operator()(int i, int j) const				{ return m[i][j]; }
	float&			operator()(int i, int j)					{ return m[i][j]; }

	Matrix2&		operator=(const Matrix2& _m);

	Matrix2		    operator*(const Matrix2& _m) const;
	Matrix2&		operator*=(const Matrix2& _m);

	void			createIdentity();

	void			scale(float f);
	void			scale(float fx, float fy);

	void			rotation(float angle);

	void			rotationScale(float angle, float scale);
	void			rotationScale(float angle, float xscale, float yscale);

	bool			inverse(Matrix2& _m) const;
};

inline ostream& operator << (ostream& os, const Matrix2& m)
{
	for(int i=0;i<2;++i)
		os << "| " << m(i,0) << " , " << m(i,1) << " |" << endl;
	return os;
}

}
