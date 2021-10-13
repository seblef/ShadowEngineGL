
#version 420 core
#include "SceneInfos.glsl"
#include "Common.glsl"

in	vec4	v_worldPos;
in	vec3	v_normal;
in	vec2	v_texBase;
in	mat3	v_tangent;

out	vec4	f_color[2];

void main(void)
{
	vec3 color = texture(tDiffuse, v_texBase).rgb * matDiffuse.rgb;
	vec2 specular = texture(tSpecular, v_texBase).rg;
	vec3 normal = texture(tNormal, v_texBase).xyz;

	normal = normalize(2.f * (normal - 0.5f));
	normal = normalize(v_tangent * normal) * 0.5f + 0.5f;

	float shininess = matShininess / 255.f;

	f_color[0]=vec4(color, matSpecularIntensity);
	f_color[1]=vec4(normal, shininess);
}
