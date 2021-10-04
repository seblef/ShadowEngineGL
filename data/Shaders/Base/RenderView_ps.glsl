
#version 420 core

in vec2		v_uv;

out vec4	f_color;

layout(binding=0)	uniform sampler2D	tBuffer;

void main(void)
{
	f_color = texture(tBuffer, v_uv);
}

