
#version 420 core

layout(std140, binding=0) uniform CopyInfo
{
	vec4		copy_color;
};

in vec2		v_uv;

out vec4	f_color;

layout(binding=0)	uniform sampler2D	tDiffuse;
layout(binding=1)	uniform sampler2D	tNormal;
layout(binding=2)	uniform sampler2D	tDepth;

void main(void)
{
	vec4	dif=texture(tDiffuse,v_uv) * copy_color.r;
	vec4	norm=texture(tNormal,v_uv) * copy_color.g;
	float	depth=texture(tDepth,v_uv).r * copy_color.b;

	f_color=abs(dif + norm + vec4(depth,depth,depth,1));
}

