
#version 420 core
#include "HDR/HDRCommon.glsl"

ivec2 i2Offset[4] = {
	ivec2(0,0),
	ivec2(0,1),
	ivec2(1,0),
	ivec2(1,1)
};

in	vec2	v_uv;

out	vec4	f_color;

void main(void)
{
	vec3 col=vec3(0,0,0);
	float grey;

	for(int i=0;i<4;++i)
		col+=textureOffset( tSrc, v_uv, i2Offset[i]).rgb;

	col*=0.25f;
	col=max(col - brightThreshold, 0.0f);

	f_color=vec4(col,1);
}

