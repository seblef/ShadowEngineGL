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
	mat3 rot = mat3(worldMat);

	vec4 localPos = getLocalPosition();

	v_worldPos = viewProj * localPos;

	v_normal = getLocalNormalBinormalTangent(norm,rot);
	vec3 t = getLocalNormalBinormalTangent(tangent,rot);
	t = normalize(t - dot(t, v_normal) * v_normal);
	vec3 b = cross(v_normal, t);
	
	v_texBase = vec2(dot(matUVRotX,uv), dot(matUVRotY,uv));
	v_texBase *= matUVScale;
	v_texBase += matUVOffset;

	v_tangent = mat3(t,b,v_normal);

	gl_Position = v_worldPos;
}

