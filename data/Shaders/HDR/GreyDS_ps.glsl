
#version 420 core
#include "HDR/HDRCommon.glsl"

ivec2 i2Offset[9]={
	ivec2(-1,-1),
	ivec2( 0,-1),
	ivec2( 1,-1),
	ivec2(-1, 0),
	ivec2( 0, 0),
	ivec2( 1, 0),
	ivec2(-1, 1),
	ivec2( 0, 1),
	ivec2( 1, 1)
};

in	vec2		v_uv;

out	vec4		f_color;

void main(void)
{
	float avg=0.0f, mx=-1e20f;
	vec4 col;
	
	for(int i=0;i<9;++i)
	{
		col=textureOffset(tSrc, v_uv, i2Offset[i]);
		avg+=col.r;
		mx=max(mx,col.g);
	}

	f_color=vec4(avg/9.0f,mx,0,0);
}

