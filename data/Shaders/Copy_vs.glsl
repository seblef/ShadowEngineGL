
#version 420 core

layout(location=0) in vec3 pos_model;
layout(location=2) in vec2 modelUV;

out vec2 uv;

void main(void)
{
	gl_Position.xyz=pos_model;
	gl_Position.w=1.0f;

    uv.x=modelUV.x;
    uv.y=1 - modelUV.y;
}
