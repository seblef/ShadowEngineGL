
#pragma once

#include "TAnimTrack.h"
#include "TAnimMultiTrack.h"
#include "Vector4.h"
#include "AnimPRSKeyVal.h"

namespace Core
{

class AnimBoolEvaluator
{
public:
	static void		evaluate(bool prev, bool next, float time, bool& val);
};

class AnimIntEvaluator
{
public:
	static void		evaluate(int prev, int next, float time, int& val);
};

class AnimFloatEvaluator
{
public:
	static void		evaluate(float prev, float next, float time, int& val);
};

class AnimVector3Evaluator
{
public:

	static void		evaluate(const Vector3& prev, const Vector3& next, float time, Vector3& val);
};

class AnimVector4Evaluator
{
public:
	static void		evaluate(const Vector4& prev, const Vector4& next, float time, Vector4& val);
};

class AnimPRSEvaluator
{
public:
	static void		evaluate(const AnimPRSKeyVal& prev, const AnimPRSKeyVal& next, float time, AnimPRSKeyVal& val);
};

typedef TAnimKey<bool>									AnimBoolKey;
typedef TAnimTrack<bool,AnimBoolEvaluator>				AnimBoolTrack;
typedef TAnimMultiTrack<bool,AnimBoolEvaluator>			AnimBoolMultiTrack;

typedef TAnimKey<int>									AnimIntKey;
typedef TAnimTrack<int,AnimIntEvaluator>				AnimIntTrack;
typedef TAnimMultiTrack<int,AnimIntEvaluator>			AnimIntMultiTrack;

typedef TAnimKey<float>									AnimFloatKey;
typedef TAnimTrack<float,AnimFloatEvaluator>			AnimFloatTrack;
typedef TAnimMultiTrack<float,AnimFloatEvaluator>		AnimFloatMultiTrack;

typedef TAnimKey<Vector3>								AnimVector3Key;
typedef TAnimTrack<Vector3,AnimVector3Evaluator>		AnimVector3Track;
typedef TAnimMultiTrack<Vector3,AnimVector3Evaluator>	AnimVector3MultiTrack;

typedef TAnimKey<Vector4>								AnimVector4Key;
typedef TAnimTrack<Vector4,AnimVector4Evaluator>		AnimVector4Track;
typedef TAnimMultiTrack<Vector4,AnimVector4Evaluator>	AnimVector4MultiTrack;

typedef TAnimKey<AnimPRSKeyVal>							AnimPRSKey;
typedef TAnimTrack<AnimPRSKeyVal,AnimPRSEvaluator>		AnimPRSTrack;
typedef TAnimMultiTrack<AnimPRSKeyVal,AnimPRSEvaluator>	AnimPRSMultiTrack;

};
