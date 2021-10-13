
#ifndef _LIGHTCOMMON_H_
#define _LIGHTCOMMON_H_

layout(binding=0)	uniform sampler2D	tAldebo;
layout(binding=1)	uniform sampler2D	tNormal;
layout(binding=2)	uniform sampler2D	tDepth;
layout(binding=3)	uniform sampler2D	tSSAO;
layout(binding=4)	uniform sampler2D	tAccumulation;
layout(binding=5)	uniform sampler2D	tHDAO;


float getSpecularFact(
	vec3 normal,
	vec3 pointToLight,
	vec3 point,
	float specularIntensity,
	float shininess
)
{
	vec3 r = reflect(pointToLight, normal);
	vec3 v = normalize( eyePos.xyz - point );

	return pow(max(dot(r, v), 0), shininess) * specularIntensity;
}


const vec3 luminanceVector = vec3(0.2126f, 0.7152f, 0.0722f);


vec3 getSpecularColor(vec3 rgb, float lum)
{
	return rgb * lum;
	// float rgblum = dot(rgb, luminanceVector);
	// return rgb * lum / (rgblum + 1e-6f);
}

#endif
