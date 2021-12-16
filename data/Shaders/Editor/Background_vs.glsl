
#version 420 core
#include "vx2d.glsl"

out vec2		v_uv;

void main(void)
{
	v_uv=uv;
	gl_Position=vec4(pos,1);
}
