
#version 420 core

layout(binding=0)	uniform sampler2D	tDiffuse;

in	vec4		v_color;
in	vec2		v_uv;

out	vec4		f_color;

void main(void)
{
	f_color=texture( tDiffuse, v_uv) * v_color;
}


