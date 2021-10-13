
#ifndef _LIGHTINFOS_H_
#define _LIGHTINFOS_H_

layout(std140, binding=2)	uniform		LightInfos
{
	vec4		lightColor;
	vec4		lightPos;
	vec4		lightDir;
	float		nearAngle;
	float		farAngle;
	float		lightRange;
	float		invRange;
	mat4		areaViewProj;
};

float getRangeAttenuation(float dist)
{
	float r = max(lightRange - dist, 0.0f) * invRange;
	return r * r * (3.0f - 2.0f * r);
}

#endif
