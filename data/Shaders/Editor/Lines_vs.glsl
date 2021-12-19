#version 420 core

#include "SceneInfos.glsl"
#include "vxbase.glsl"

out vec4 v_color;

void main(void)
{
    v_color = color;
    gl_Position = viewProj * (worldMat * vec4(pos,1));
}
