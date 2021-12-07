#include "AnimPRSKeyVal.h"

using namespace Core;

AnimPRSKeyVal::AnimPRSKeyVal(const Vector3& pos, const Matrix3& rotScale) : 
    _pos(pos),
    _rotScale(rotScale)
{
}

AnimPRSKeyVal::AnimPRSKeyVal(const Matrix4& m) : _pos(m)
{
    m.getMatrix3(_rotScale);
}

AnimPRSKeyVal::AnimPRSKeyVal(const AnimPRSKeyVal& k) :
    _pos(k._pos),
    _rotScale(k._rotScale)
{
}

AnimPRSKeyVal& AnimPRSKeyVal::operator=(const AnimPRSKeyVal& k)
{
    _pos = k._pos;
    _rotScale = k._rotScale;
    return *this;
}

AnimPRSKeyVal& AnimPRSKeyVal::operator=(const Matrix4& m)
{
    _pos = m;
    m.getMatrix3(_rotScale);
    return *this;
}

void AnimPRSKeyVal::setPosition(const Vector3& pos)
{
    _pos = pos;
}

void AnimPRSKeyVal::setRotationScale(const Matrix3& rotScale)
{
    _rotScale = rotScale;
}

void AnimPRSKeyVal::getMatrix4(Matrix4& m) const
{
    m = _rotScale;
    m = _pos;
}