
#version 420 core
#include "SceneInfos.glsl"
#include "Common.glsl"

in	vec4	v_worldPos;
in	vec3	v_normal;
in	vec2	v_texBase;
in	mat3	v_tangent;

out	vec4	f_color;

void main(void)
{
	vec3 normal, I, r;
	vec4 env, emissive;

	emissive=texture( tDiffuse, v_texBase);
	normal=(texture( tNormal, v_texBase).rgb - 0.5f) * 2.0f;
	normal=v_tangent * normal;

	I=v_worldPos.xyz - eyePos.xyz;
	r=reflect(I,normal);

	env=texture( tEnvironment, r.xy);
	f_color=emissive + env * matReflectivity;
}

