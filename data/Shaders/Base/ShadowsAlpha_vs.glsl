
#version 420 core
#include "vx3d.glsl"
#include "SceneInfos.glsl"
#include "Base/ShadowsCommon.glsl"

out	vec2	oUV;
out	vec2	oZ;

void main(void)
{
	vec4 p=viewProj * (worldMat * vec4(pos,1));
	oUV=uv;
	oZ=p.zw;

	gl_Position=p;
}

