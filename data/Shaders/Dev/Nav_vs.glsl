
#version 420 core
#include "vxbase.glsl"
#include "SceneInfos.glsl"

out	vec4	v_color;

void main(void)
{
	gl_Position=viewProj * vec4(pos,1);
	v_color=color;
}

