
#version 420 core
#include "Base/ShadowsCommon.glsl"

in	vec2	oUV;
in	vec2	oZ;

out	vec4	f_color;

void main(void)
{
	vec4 rgba=texture( tDiffuse, oUV);
	if(rgba.a < ALPHA_THRESHOLD)
		discard;

	f_color=vec4(oZ.x / oZ.y,0.0f,0.0f,1.0f);
}

