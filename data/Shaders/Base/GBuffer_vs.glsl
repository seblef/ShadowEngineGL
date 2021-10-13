
#version 420 core
#include "vx3d.glsl"
#include "SceneInfos.glsl"
#include "Common.glsl"

out	vec4	v_worldPos;
out	vec3	v_normal;
out	vec2	v_texBase;
out	mat3	v_tangent;


void main(void)
{
	mat3 rot=mat3(worldMat);

	v_worldPos = viewProj * (worldMat * vec4(pos,1));
	v_normal = normalize(rot * norm);
	
	v_texBase = vec2(dot(matUVRotX,uv), dot(matUVRotY,uv));
	v_texBase *= matUVScale;
	v_texBase += matUVOffset;

	vec3 t = normalize(rot * tangent);
	t = normalize(t - dot(t, v_normal) * v_normal);
	vec3 b = cross(v_normal, t);

	v_tangent = mat3(t,b,v_normal);
	gl_Position = v_worldPos;
}

