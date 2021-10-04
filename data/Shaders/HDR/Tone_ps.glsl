
#version 420 core
#include "HDR/HDRCommon.glsl"

in	vec2		v_uv;

out	vec4		f_color;

void main(void)
{
	vec3 color = texture(tSrc, v_uv).rgb;
    vec3 mapped = vec3(1.0) - exp(color * exposure);
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

	f_color = vec4(mapped, 1);
}

