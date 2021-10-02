
#version 420 core
#include "HDR/HDRCommon.glsl"

ivec2 i2Offset[9]={
	ivec2(-4,0),
	ivec2(-3,0),
	ivec2(-2,0),
	ivec2(-1,0),
	ivec2( 0,0),
	ivec2( 1,0),
	ivec2( 2,0),
	ivec2( 3,0),
	ivec2( 4,0)
};

in	vec2	v_uv;

out	vec4	f_color;

void main(void)
{
	f_color=vec4(0,0,0,0);
	for(int i=0;i<9;++i)
		f_color+= textureOffset(tSrc, v_uv, i2Offset[i]) * gaussianWeights[i];
}

