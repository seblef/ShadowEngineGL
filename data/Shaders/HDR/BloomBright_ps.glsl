
#version 420 core
#include "HDR/HDRCommon.glsl"

in	vec2		v_uv;

out	vec4		f_color;

void main(void)
{
    vec3 color = texture(tSrc, v_uv).rgb;
    float luminance = dot(color, luminanceVector);
    
    if(luminance < brightnessThreshold)
    {
        f_color = vec4(0,0,0,1);
    }
    else
    {
        f_color = vec4(color * (luminance - brightnessThreshold) / (1 - brightnessThreshold), 1);
    }
}
