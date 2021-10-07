
#ifndef _HDRCOMMON_H_
#define _HDRCOMMON_H_

#include "SceneInfos.glsl"

layout(std140, binding=4)	uniform		BrightInfos
{
	vec2 invFrameSize;
	float gamma;
	float exposure;
	float brightnessThreshold;
	float radius;
	float strength;
	float fpack;
};


layout(binding=0)	uniform sampler2D	tSrc;


vec3 luminanceVector  = vec3(0.30f,0.59f,0.11f);



vec2 bloomDSFilterOffsets[13] = {
	vec2(-2, -2),
	vec2(0, -2),
	vec2(2, -2),
	vec2(-1, -1),
	vec2(1, -1),
	vec2(-2, 0),
	vec2(0, 0),
	vec2(2, 0),
	vec2(-1, 1),
	vec2(1, 1),
	vec2(-2, 2),
	vec2(0, 2),
	vec2(2, 2)
};

vec2 bloomUSFilterOffsets[9] = {
	vec2(-1, 1),
	vec2(0, -1),
	vec2(1, -1),
	vec2(-1, 0),
	vec2(0, 0),
	vec2(1, 0),
	vec2(-1, 1),
	vec2(0, 1),
	vec2(1, 1)
};



vec3 boxAverage(in vec3 v1, in vec3 v2, in vec3 v3, in vec3 v4)
{
	return ((v1 + v2 + v3 + v4) * 0.25f);
}

vec3 bloomDownSample(in sampler2D src, in vec2 uv, in vec2 scale)
{
	vec3 s[13];
	vec2 offset = scale * 0.5f;
	for(int i=0;i<13;++i)
		s[i] = texture(src, uv + bloomDSFilterOffsets[i] * scale + offset).rgb;

	return(boxAverage(s[0], s[1], s[5], s[6]) * 0.125f +
		   boxAverage(s[5], s[6], s[10], s[11]) * 0.125f +
		   boxAverage(s[1], s[2], s[6], s[7]) * 0.125f +
		   boxAverage(s[6], s[7], s[11], s[12]) * 0.125f +
		   boxAverage(s[3], s[4], s[8], s[9]) * 0.5f);
}

vec3 bloomUpSample(in sampler2D src, in vec2 uv, in vec2 scale)
{
	vec3 s[9];
	vec2 offset = scale * 0.5f;

	for (int i=0;i<9;++i)
		s[i] = texture(src, uv + bloomUSFilterOffsets[i] * scale + offset).rgb;

	return (0.0625f * (
		s[0] +
		s[1] * 2.f +
		s[2] +
		s[3] * 2.f +
		s[4] * 4.f +
		s[5] * 2.f +
		s[6] +
		s[7] * 2.f +
		s[8]
	));
}

#endif
