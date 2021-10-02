
#version 420 core
#include "HDR/HDRCommon.glsl"

layout(binding=1)	uniform sampler2D	tLum;
layout(binding=2)	uniform sampler2D	tBloom;

in	vec2		v_uv;

out	vec4		f_color;

void main(void)
{
	vec4 col,lum,bloom;
	float lp, lmsqr, toneScalar;

	col=texture( tSrc, v_uv);
	lum=texelFetch(tLum, ivec2(0,0), 0);
	bloom=texture( tBloom, v_uv) * 0.5f;

	lp=(exposure / lum.r) * dot(col.rgb,luminanceVector);
	lmsqr=lum.g * (1.0f + gaussianScalar);
	lmsqr*=lmsqr;

	toneScalar=(lp * (1.0f + (lp / (lmsqr + 0.001f)))) / (1.0f + lp);

	f_color=col * toneScalar + bloom;
}

