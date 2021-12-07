
#pragma once

#include "Vector3.h"

namespace Core
{

class AnimPRSKeyVal
{
protected:

	Vector3				_pos;
	Matrix3				_rotScale;

public:

	AnimPRSKeyVal()		{}
	AnimPRSKeyVal(const Vector3& pos, const Matrix3& rotScale);
	AnimPRSKeyVal(const Matrix4& m);
	AnimPRSKeyVal(const AnimPRSKeyVal& k);

	AnimPRSKeyVal&		operator=(const AnimPRSKeyVal& k);
	AnimPRSKeyVal&		operator=(const Matrix4& m);

	void				setPosition(const Vector3& pos);
	void				setRotationScale(const Matrix3& rotScale);

	const Vector3&		getPosition() const									{ return _pos; }
	const Matrix3&		getRotationScale() const							{ return _rotScale; }

	void				getMatrix4(Matrix4& m) const;
};

};
