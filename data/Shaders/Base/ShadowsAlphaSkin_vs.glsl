
#version 420 core
#include "vxskin.glsl"
#include "SceneInfos.glsl"
#include "Base/ShadowsCommon.glsl"
#include "SkinCommon.glsl"

out	vec2	oUV;
out	vec2	oZ;


void main(void)
{
	vec4 localPos,wp;
	getLocalPosition(localPos);

	wp=viewProj * localPos;
	oUV=uv;
	oZ=wp.zw;

	gl_Position=wp;
}

