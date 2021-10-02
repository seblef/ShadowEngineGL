
#ifndef _LIGHTCOMMON_H_
#define _LIGHTCOMMON_H_

layout(binding=0)	uniform sampler2D	tAldebo;
layout(binding=1)	uniform sampler2D	tNormal;
layout(binding=2)	uniform sampler2D	tDepth;
layout(binding=3)	uniform sampler2D	tSSAO;
layout(binding=4)	uniform sampler2D	tAccumulation;
layout(binding=5)	uniform sampler2D	tHDAO;


void	getSpecularFact(in vec3 normal, in vec3 pointToLight, in vec3 point, in float specInt, in float shine, out float spec)
{
	vec3 r=( normal * (2.0f * dot( normal, pointToLight) ) ) - pointToLight;
	vec3 v=normalize( eyePos.xyz - point );

	spec=pow( max( dot(r,v), 0), shine) * specInt;
}

void	getLuminance(in vec3 rgb, out float lum)
{
	lum= 0.2126f * rgb.r + 0.7152f * rgb.g + 0.0722f * rgb.b;
}

void	getSpecularColor(in vec3 rgb, in float lum, out vec3 spec)
{
	float rgblum;
	getLuminance(rgb, rgblum);
	spec=rgb * lum / (rgblum + 1e-6f);
}

#endif
