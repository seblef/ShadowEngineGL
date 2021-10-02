
#ifndef _GCOMMON_H_
#define _GCOMMON_H_

layout(std140, binding=2)	uniform		MaterialInfos
{
	vec4		matAmbient;
	vec4		matDiffuse;
	vec4		matSpecular;
	vec4		matShininess;

	vec2		matUVOffset;
	vec2		matUVScale;
	vec2		matUVRotX;
	vec2		matUVRotY;
};

layout(binding=0)	uniform sampler2D	tDiffuse;
layout(binding=1)	uniform sampler2D	tNormal;
layout(binding=2)	uniform sampler2D	tSpecular;
layout(binding=3)	uniform sampler2D	tEnvironment;


//			Add pass: tDiffuse = Emissive Texture


#endif
