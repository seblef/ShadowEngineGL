
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
	float grey=0.0f, grey_val, mx=-1e20f;
	vec4 col;

	for(int i=0;i<4;++i)
	{
		grey_val=dot(textureOffset( tSrc, v_uv, i2Offset[i]).rgb, luminanceVector);
		grey+=grey_val;

		mx=max(mx, grey_val);
	}

	f_color=vec4(grey*0.25f,mx,0,0);
}


