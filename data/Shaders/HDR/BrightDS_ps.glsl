
#version 420 core
#include "HDR/HDRCommon.glsl"

ivec2 i2Offset[16]={
	ivec2(0,0),
	ivec2(0,1),
	ivec2(0,2),
	ivec2(0,3),
	ivec2(1,0),
	ivec2(1,1),
	ivec2(1,2),
	ivec2(1,3),
	ivec2(2,0),
	ivec2(2,1),
	ivec2(2,2),
	ivec2(2,3),
	ivec2(3,0),
	ivec2(3,1),
	ivec2(3,2),
	ivec2(3,3)
};

in	vec2	v_uv;

out	vec4	f_color;

void main(void)
{
	f_color=vec4(0,0,0,0);

	for(int i=0;i<16;++i)
		f_color+=textureOffset(tSrc, v_uv, i2Offset[i]);

	f_color/=16.0f;
}

