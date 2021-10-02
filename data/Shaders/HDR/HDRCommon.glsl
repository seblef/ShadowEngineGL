
#ifndef _HDRCOMMON_H_
#define _HDRCOMMON_H_

#include "SceneInfos.glsl"

layout(std140, binding=4)	uniform		BrightInfos
{
	vec2		invBloomTex;
	vec2		greyScaleUV;
	float		brightThreshold;
	float		exposure;
	float		gaussianScalar;
	float		brightBack;
};


layout(binding=0)	uniform sampler2D	tSrc;


float gaussianWeights[9]={
	0.091325f,
	0.128537f,
	0.164080f,
	0.189965f,
	0.199471f,
	0.189965f,
	0.164080f,
	0.128537f,
	0.091325f
};

vec3 luminanceVector  = vec3(0.30f,0.59f,0.11f);


#endif
