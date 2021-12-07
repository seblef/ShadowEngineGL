
#pragma once

#include "Plane.h"
#include <assert.h>

#define FRUSTRUM_MAX_PLANES				32
#define FRUSTRUM_MAX_USER_PLANES		30

namespace Core
{

enum ViewFrustumPlane
{
	VFP_LEFT=0,
	VFP_RIGHT,
	VFP_BOTTOM,
	VFP_TOP,
	VFP_FAR,
	VFP_NEAR
};

class ViewFrustum
{
	protected:

		bool				_builtFromMatrix;
		int					_planeCount;

		Vector3				_position;
		Matrix4				_viewProj;
		Plane				_planes[FRUSTRUM_MAX_PLANES];

	public:

		ViewFrustum();
		ViewFrustum(const Matrix4& mat);
		ViewFrustum(const ViewFrustum& vf);

		ViewFrustum&		operator=(const ViewFrustum& vf);

		void				reinit();

		void				setPosition(const Vector3& vPos);
		const Vector3&		getPosition() const;

		void				transform(const Matrix4& mat);
		const Matrix4&		getViewProjMatrix() const;

		void				createFromViewProjMatrix(const Matrix4& mat, bool bNearAndFar=true);

		void				addPlane(const Plane& p);
		void				addPlane(const Vector3& v1, const Vector3& v2);

		int					getPlaneCount() const;
		const Plane&		getPlane(int n) const;

		bool				isPointInside(const Vector3& v) const;
		bool				isLineInside(const Vector3& start, const Vector3& end) const;
		bool				isBoxInside(const BBox3& v) const;
		bool				isSphereInside(const BSphere& s) const;

		bool				isMatrixCreated() const;

		Vector3				getFarLeftUp() const;
		Vector3				getFarLeftDown() const;
		Vector3				getFarRightUp() const;
		Vector3				getFarRightDown() const;
};

}
