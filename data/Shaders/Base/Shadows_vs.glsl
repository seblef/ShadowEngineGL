
#version 420 core
#include "vx3d.glsl"
#include "SceneInfos.glsl"
#include "Base/ShadowsCommon.glsl"

out	vec2	v_z;

void main(void)
{
	vec4 p=viewProj * (worldMat * vec4(pos,1));
	v_z=p.zw;

	gl_Position=p;
}

