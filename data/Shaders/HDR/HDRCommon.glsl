
#ifndef _HDRCOMMON_H_
#define _HDRCOMMON_H_

#include "SceneInfos.glsl"

layout(std140, binding=4)	uniform		BrightInfos
{
	float gamma;
	float exposure;
	vec2 f2pack;
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
