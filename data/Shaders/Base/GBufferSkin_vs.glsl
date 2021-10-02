#version 420 core
#include "vxskin.glsl"
#include "SceneInfos.glsl"
#include "Common.glsl"
#include "SkinCommon.glsl"

out	vec4	v_worldPos;
out	vec3	v_normal;
out	vec2	v_texBase;
out	mat3	v_tangent;


void main(void)
{
	vec4 localPos;
	vec3 t,b;
	mat3 rot=mat3(worldMat);

	getLocalPosition(localPos);

	v_worldPos=viewProj * localPos;

	getLocalNormalBinormalTangent(norm,rot,v_normal);
	getLocalNormalBinormalTangent(tangent,rot,t);
	getLocalNormalBinormalTangent(binormal,rot,b);
	
	v_texBase=vec2(dot(matUVRotX,uv), dot(matUVRotY,uv));
	v_texBase*=matUVScale;
	v_texBase+=matUVOffset;

	v_tangent=mat3(t,b,v_normal);

	gl_Position=v_worldPos;
}

