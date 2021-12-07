
#pragma once

#include "Vector3.h"


namespace Core
{

class BSphere;

class BBox3
{
protected:

	Vector3			_min,_max;

public:

	static BBox3	NullBox;
	static BBox3	BoundBox;
	static BBox3	InitBox;

	BBox3();
	BBox3(const Vector3& min, const Vector3& max);
	BBox3(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);
	BBox3(const BBox3& b);
	BBox3(const BSphere& b);
	~BBox3() {}

	void			createCenterSize(const Vector3& c, const Vector3& s);

	const Vector3&	getMin() const;
	const Vector3&	getMax() const;
	void			getMinMax(Vector3& min, Vector3& max) const;

	void			setMin(const Vector3& min);
	void			setMax(const Vector3& max);
	void			setMinMax(const Vector3& min, const Vector3& max);

	Vector3			getCenter() const;
	void			getCenter(Vector3& c) const;

	Vector3			getSize() const;
	void			getSize(Vector3& s) const;

	BBox3&			operator=(const BBox3& b);
	BBox3&			operator=(const BSphere& b);

	BBox3			operator+(const Vector3& v) const;
	BBox3			operator-(const Vector3& v)	const;

	BBox3&			operator+=(const Vector3& v);
	BBox3&			operator-=(const Vector3& v);

	BBox3&			operator << (const Vector3& v);
	BBox3&			operator << (const BBox3& b);
	BBox3&			operator << (const BSphere& b);

	BBox3			operator+(const BBox3& b) const;
	BBox3&			operator+=(const BBox3& b);

	BBox3			operator*(const Matrix4& m) const;
	BBox3&			operator*=(const Matrix4& m);

	void			transformFromBox(const BBox3& b, const Matrix4& m);

	bool			isValid() const;
	void			repair();

	void			getSphere(BSphere& b) const;

	bool			isIn(const Vector3& v) const;
	bool			isIn(const BBox3& b) const;
	bool			isIn(const Vector3& c, float r) const;

	bool			isCompletlyIn(const BBox3& b) const;
	bool			isCompletlyIn(const Vector3& c, float r) const;

	bool			rayIntersect(const Vector3& o, const Vector3& d) const;

	float			getDistanceFromSq(const Vector3& p) const;
	float			getDistanceFrom(const Vector3& p) const;

	void			getPoints(Vector3 *pv) const;
};

inline ostream& operator << (ostream& os, const BBox3& b)
{
	os << b.getMin() << " <--> " << b.getMax();
	return os;
}

};
