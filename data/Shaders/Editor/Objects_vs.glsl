#version 420 core
#include "SceneInfos.glsl"
#include "vx3d.glsl"

out vec2 v_uv;

void main(void)
{
    v_uv = uv;
    gl_Position = viewProj * (worldMat * vec4(pos,1));
}
