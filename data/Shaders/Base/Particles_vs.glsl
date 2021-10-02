
#version 420 core
#include "vxbase.glsl"
#include "SceneInfos.glsl"

out	vec4		v_color;
out	vec2		v_uv;


void main(void)
{
	gl_Position=viewProj * vec4(pos,1);
	gl_Position.y=1.0f-gl_Position.y;
	v_uv=uv;
	v_color=color;
}

