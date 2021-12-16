#version 420 core

#include "Common.glsl"
#include "Editor/Material.glsl"

in	vec4	v_worldPos;
in	vec3	v_normal;
in	vec2	v_texBase;
in	mat3	v_tangent;

out	vec4	f_color;


float getSpecularFact(
	vec3 normal,
	vec3 pointToLight,
	vec3 point,
	float specularIntensity,
	float shininess
)
{
	vec3 r = reflect(pointToLight, normal);
	vec3 v = normalize( eyePos.xyz - point );

	return pow(max(dot(r, v), 0), shininess) * specularIntensity;
}


void main(void)
{
	vec3 color = texture(tDiffuse, v_texBase).rgb;
    vec3 aldebo = color * matDiffuse.rgb;
    vec3 final = color * matAmbient.rgb;
	vec3 normal = texture(tNormal, v_texBase).xyz;

	normal = normalize(2.f * (normal - 0.5f));
	normal = normalize(v_tangent * normal) * 0.5f + 0.5f;

	float NdotL = -dot(normal, lightDir.xyz);
	if(NdotL < 0.0f)
	{
		f_color = vec4(final,1);
		return;
	}

	vec3 lightIntensity = lightColor.rgb * lightColor.a;
	final += lightIntensity * aldebo * NdotL;

	final += lightIntensity * getSpecularFact(
		normal, lightDir.xyz, v_worldPos.xyz, matSpecularIntensity, matShininess
	);

	f_color = vec4(final, 1);
}
