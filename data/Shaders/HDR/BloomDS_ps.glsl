
#version 420 core
#include "HDR/HDRCommon.glsl"

in	vec2		v_uv;

out	vec4		f_color;

void main(void)
{
    vec3 out_color = bloomDownSample(tSrc, v_uv, invFrameSize);
    f_color = vec4(out_color, 1);
}
