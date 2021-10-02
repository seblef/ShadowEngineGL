
#version 420 core

in	vec2	oUV;
in	vec2	oZ;

out	vec4	f_color;

void main(void)
{
	f_color=vec4(oZ.x / oZ.y,0.0f,0.0f,1.0f);
}

