#ifndef _MATERIALED_H_
#define _MATERIALED_H_

layout(std140, binding=0) 	uniform 	SceneInfos
{
	mat4				viewProj;
	vec4				eyePos;
    vec4                lightDir;
    vec4                lightColor;
};

#endif