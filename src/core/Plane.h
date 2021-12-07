
#pragma once

#include "BBox3.h"

namespace Core
{

enum PlaneSide
{
	PLANE_FRONT=0,
	PLANE_ON,
	PLANE_BACK,
	PLANE_CLIP
};

enum PlaneAxis
{
	PLANE_X=0,
	PLANE_Y,
	PLANE_Z
};


class Plane
{
protected:
		
	Vector3			_normal;
	float			_dist;

public:

	Plane()			{}
	Plane(const Plane& p);
	Plane(const Vector3& normal, float dist);
	Plane(const Vector3& point, const Vector3& norm);
	Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3);
	~Plane()		{}

	Plane&			operator=(const Plane& p);

	const Vector3&	getNormal() const;
	float			getDistance() const;

	void			recalculateDist(const Vector3& v);

	void			makePlane(const Vector3& normal, float dist);
	void			makePlane(const Vector3& point, const Vector3& norm);
	void			makePlane(const Vector3& p1, const Vector3& p2, const Vector3& p3);

	float			getDistanceFrom(const Vector3& p) const;

	bool			intersectLine(const Vector3& o, const Vector3& d, float& t) const;
	bool			intersectLine(const Vector3& o, const Vector3& d, Vector3& p) const;

	bool			intersectEdge(const Vector3& v1, const Vector3& v2, Vector3& p) const;

	PlaneSide		classifyPoint(const Vector3& p) const;
	PlaneSide		classifySphere(const BSphere& b) const;
	PlaneSide		classifyBox(const BBox3& b) const;

	Vector3			getPoint() const;
	void			getPoint(Vector3& p) const;

	bool			existsPlaneIntersection(const Plane& p) const;
	bool			intersectPlane(const Plane& p, Vector3& o, Vector3& d) const;
	bool			intersectWithPlanes(const Plane& p1, const Plane& p2, Vector3& it) const;

	bool			isFront(const Vector3& look_dir) const;

	Plane			operator*(const Matrix4& mat) const;
	Plane&			operator*=(const Matrix4& mat);
};

}
