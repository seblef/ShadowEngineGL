
#version 420 core
#include "vx2d.glsl"

out vec4		v_color;
out vec2		v_uv;

void main(void)
{
	v_uv=uv;
	v_color=color;
	gl_Position=vec4(pos,1);
}

