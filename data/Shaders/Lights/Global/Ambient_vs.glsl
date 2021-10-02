
#version 420 core
#include "vxbase.glsl"

out vec2 v_uv;

void main(void)
{
	gl_Position.xyz=pos;
	gl_Position.w=1.0f;

	v_uv=uv;
}

