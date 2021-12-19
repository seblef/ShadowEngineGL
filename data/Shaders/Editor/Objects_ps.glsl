#version 420 core

layout(binding=0)	uniform sampler2D	tDiffuse;

in vec2 v_uv;

out vec4 f_color;

void main(void)
{
    vec3 color = texture(tDiffuse, v_uv).rgb;
    f_color = vec4(color, 1);
}