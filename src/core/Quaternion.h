
#pragma once

#include "Vector3.h"

namespace Core
{

class Quaternion
{
	public:

		float				x,y,z,w;

		Quaternion() : x(0.0f),y(0.0f),z(0.0f),w(1.0f) {}
		Quaternion(float _x, float _y, float _z, float _w) : x(_x),y(_y),z(_z),w(_w) {}
		Quaternion(const Quaternion& quat) : x(quat.x),y(quat.y),z(quat.z),w(quat.w) {}
		Quaternion(const Matrix4& mat)									{ buildMatrix(mat); }
		Quaternion(const Matrix3& mat)									{ buildMatrix(mat); }
		Quaternion(const Vector3& vAxis, const float fAngle)			{ buildAxisAngle(vAxis,fAngle); }
		~Quaternion() {}

		void				buildMatrix(const Matrix4& mat);
		void				buildMatrix(const Matrix3& mat);
		void				buildAxisAngle(const Vector3& axis, float fAngle);
		void				buildEuler(const Vector3& vRot)				{ buildEuler(vRot.x,vRot.y,vRot.z); }
		void				buildEuler(float ex, float ey, float ez);
		void				buildRotationArc(const Vector3& vFrom, const Vector3& vTo);

		void				getMatrix(Matrix4& mat) const;
		void				getMatrix(Matrix3& mat) const;
		void				getAxisAngle(Vector3& vAxis, float &fAngle) const;
		Quaternion			getInverse() const							{ return Quaternion(-x,-y,-z,w); }
		void				getInverse(Quaternion& q) const				{ q.x=-x; q.y=-y; q.z=-z; q.w=w; }

		void				normalize();
		void				reset() { x=y=z=0.0f; w=1.0f; }
		void				inverse() { x=-x; y=-y; z=-z; }

		Vector3				operator*(const Vector3& v) const;

		bool				operator==(const Quaternion& quat) const	{	return (x==quat.x) && (y==quat.y) && (z==quat.z) && (w==quat.w); }
		bool				operator!=(const Quaternion& quat) const	{	return (x!=quat.x) || (y!=quat.y) || (z!=quat.z) || (w!=quat.w); }

		Quaternion&			operator=(const Quaternion& quat)			{ x=quat.x; y=quat.y; z=quat.z; w=quat.w; return *this; }

		static void			align(const Quaternion& q1, const Quaternion& q2, Quaternion& qRet);
		static void			mult(const Quaternion& q1, const Quaternion& q2, Quaternion& qRet);
		static void			slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& qRet);

};

inline Vector3 operator*(const Vector3& v, const Quaternion& q)
{
	return q*v;
}

}
