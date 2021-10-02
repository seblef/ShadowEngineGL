
#version 330 core

in vec2		uv;

out vec3	color;

uniform sampler2D	sLinear;

void main(void)
{
	float r=texture(sLinear,uv).r;

	if(r==1.0f)
		r=0.0f;
	else
		r-=0.5f;
    color.rgb=vec3(r,r,r);
}
