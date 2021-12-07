
#pragma once

#include "Vector3.h"

namespace Core
{

class BSphere
{
protected:

	Vector3				_center;
	float				_radius;

public:

	BSphere();
	BSphere(const Vector3& center, float radius);
	BSphere(const BSphere& b);
	BSphere(const Vector3& min, const Vector3& max);
	~BSphere()		{}

	BSphere&			operator=(const BSphere& b);

	const Vector3&		getCenter() const;
	float				getRadius() const;

	void				setCenter(const Vector3& c);
	void				setRadius(float r);

	float				getDistanceFrom(const Vector3& v) const;
	float				getDistanceFrom(const BSphere& b) const;

	BSphere&			operator << (const Vector3& v);
	BSphere&			operator << (const BSphere& b);

	bool				isIn(const Vector3& p) const;
	bool				isIn(const BSphere& b) const;
	bool				isIn(const Vector3& min, const Vector3& max) const;

	bool				isCompletlyIn(const BSphere& b) const;
	bool				isCompletlyIn(const Vector3& min, const Vector3& max) const;
};

}
