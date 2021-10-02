
#version 420 core

in	vec2	v_z;

out	vec4	f_color;

void main(void)
{
	f_color=vec4(v_z.x / v_z.y *0.5f + 0.5f,0.0f,0.0f,1.0f);
}

