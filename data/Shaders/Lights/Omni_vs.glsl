
#version 420 core
#include "vxbase.glsl"
#include "SceneInfos.glsl"

void main(void)
{
	gl_Position=viewProj * (worldMat * vec4(pos,1));
}


