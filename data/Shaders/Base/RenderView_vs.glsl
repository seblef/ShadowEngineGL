

#version 420 core

#include "vx2d.glsl"

layout(std140, binding=0) 	uniform 	Offset
{
	vec4				offset;
};


out vec2 		v_uv;

void main(void)
{
	gl_Position=vec4(pos,1) + offset;
	v_uv=uv;
}

