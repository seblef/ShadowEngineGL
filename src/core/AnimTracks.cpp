#include "AnimTracks.h"

using namespace Core;


void AnimBoolEvaluator::evaluate(bool prev, bool next, float time, bool& val)
{
    val= time <= 0.5f ? prev : next;
}

void AnimIntEvaluator::evaluate(int prev, int next, float time, int& val)
{
    float fprev = (float)prev;
    float fnext = (float)next;
    val = (int)(fprev + (fnext-fprev)*time);
}

void AnimFloatEvaluator::evaluate(float prev, float next, float time, int& val)
{
    val = prev + (next-prev)*time;
}

void AnimVector3Evaluator::evaluate(const Vector3& prev, const Vector3& next, float time, Vector3& val)
{
    val = (next-prev);
    val *= time;
    val += prev;
}

void AnimVector4Evaluator::evaluate(const Vector4& prev, const Vector4& next, float time, Vector4& val)
{
    val = (next-prev);
    val *= time;
    val += prev;
}

void AnimPRSEvaluator::evaluate(const AnimPRSKeyVal& prev, const AnimPRSKeyVal& next, float time, AnimPRSKeyVal& val)
{
    Vector3 pos;
    AnimVector3Evaluator::evaluate(prev.getPosition(),next.getPosition(),time,pos);
    val.setPosition(pos);

    Matrix3 rs(next.getRotationScale());
    rs -= prev.getRotationScale();
    rs *= time;
    rs += prev.getRotationScale();
    val.setRotationScale(rs);
}