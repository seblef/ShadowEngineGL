
#pragma once

#include "Matrix2.h"
#include <fstream>

namespace Core
{

class Vector2
{
public:

	float			x,y;

	static Vector2	NullVector;
	static Vector2	XAxisVector;
	static Vector2	YAxisVector;
	static Vector2	UnitVector;

	Vector2()		{}
	Vector2(float _x, float _y) : x(_x), y(_y) {}
	Vector2(const Vector2& v) : x(v.x), y(v.y) {}

	~Vector2()		{}

	Vector2&		operator=(const Vector2& v)						{x=v.x; y=v.y; return *this; };

	bool			operator==(const Vector2& v) const				{ return ((v.x==x) && (v.y==y)); }
	bool			operator!=(const Vector2& v) const				{ return ((v.x!=x) || (v.y!=y)); }

	Vector2			operator+(const Vector2& v) const				{ return Vector2(x+v.x, y+v.y); }
	Vector2&		operator+=(const Vector2& v)					{ x+=v.x; y+=v.y; return *this; }

	Vector2			operator-() const								{ return Vector2(-x,-y); }
	Vector2			operator-(const Vector2& v) const				{ return Vector2(x-v.x, y-v.y); }
	Vector2&		operator-=(const Vector2& v)					{ x-=v.x; y-=v.y; return *this; };

	float			operator*(const Vector2& v) const				{ return x*v.x + y*v.y; }
	Vector2			operator*(float f) const						{ return Vector2(x*f, y*f); }
	Vector2&		operator*=(float f)								{ x*=f; y*=f; return *this; }
	Vector2&		operator*=(const Vector2& v)					{ x*=v.x; y*=v.y; return *this; }
	Vector2			operator*(const Matrix2& m) const;
	Vector2&		operator*=(const Matrix2& m);

	Vector2			operator/(float f) const						{ return Vector2(x/f, y/f); }
	Vector2&		operator/=(float f)								{ x/=f; y/=f; return *this; }

	float			getLength() const								{ return sqrtf(x*x + y*y); }
	float			getSqrLength() const							{ return x*x+y*y; }

	void			normalize()										{ float inv=FastInvSqrt(x*x+y*y); x*=inv; y*=inv;}	
};

inline ostream& operator << (ostream& os, const Vector2& v)
{
	os << "< " << v.x << " , " << v.y << " >";
	return os;
}

}
