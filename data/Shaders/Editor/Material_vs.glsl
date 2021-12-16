#version 420 core

#include "Common.glsl"
#include "vx3d.glsl"
#include "Editor/Material.glsl"

out	vec4	v_worldPos;
out	vec3	v_normal;
out	vec2	v_texBase;
out	mat3	v_tangent;


void main(void)
{
	v_worldPos = viewProj * vec4(pos,1);
	v_normal = normalize(norm);
	
	v_texBase = vec2(dot(matUVRotX,uv), dot(matUVRotY,uv));
	v_texBase *= matUVScale;
	v_texBase += matUVOffset;

	vec3 t = normalize(tangent);
	t = normalize(t - dot(t, v_normal) * v_normal);
	vec3 b = cross(v_normal, t);

	v_tangent = mat3(t,b,v_normal);
	gl_Position = v_worldPos;
}
