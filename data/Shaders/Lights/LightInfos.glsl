
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

void	getRangeFact(in float dist, out float r)
{
	r=max (lightRange - dist, 0.0f) * invRange;
	r= r * r * (3.0f - 2.0f * r);
}

#endif
