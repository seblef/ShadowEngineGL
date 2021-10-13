
#version 420 core
#include "SceneInfos.glsl"
#include "Lights/LightCommon.glsl"
#include "Lights/ShadowCommon.glsl"

layout(std140, binding=2)	uniform		LightInfos
{
	vec4		lightColor;
	vec4		lightDir;
	vec4		ambientLight;
};

in	vec2		v_uv;

out	vec4		f_color;

void main(void)
{
	// Extract base data
	vec4 normalData = texture(tNormal, v_uv);
	vec3 normal = normalData.xyz * 2.f - 1.f;

	vec4 colorData = texture(tAldebo, v_uv);
	vec3 aldebo = colorData.rgb;

	vec4 accumulationData = texture(tAccumulation, v_uv);
	vec3 accumulatedDiffuse = accumulationData.rgb;

	// Occlusion
	float occlusion = texture(tHDAO, v_uv).r;

	// Compute ambient ambient light
	vec3 final = aldebo * ambientLight.rgb;
	// Add accumulated diffuse
	final += accumulationData.rgb * aldebo;
	final *= occlusion;
	// Add accumulated specular
	final += getSpecularColor(accumulationData.rgb, accumulationData.a);

	// If surface isn't facing the light, exit
	float NdotL = -dot(normal, lightDir.xyz);
	if(NdotL < 0.0f)
	{
		f_color = vec4(final,1);
		return;
	}

	// Compute shadow attenuation
	float depth = texture( tDepth, v_uv).r;
	vec4 worldPos = getWorldPosition(vec3(v_uv, depth));

	float shadows = getShadowFactPoisson(worldPos);
	// If surface is completely in the shadows, exit
	if(shadows < 0.001)
	{
		f_color = vec4(final,1);
		return;
	}

	// Light color
	vec3 lightIntensity = lightColor.rgb * lightColor.a * shadows;

	// Add diffuse component
	final += lightIntensity * aldebo * NdotL * occlusion;

	// Extract specular info from gbuffer
	float shininess = normalData.a * 255.f;
	float specularIntensity = colorData.a;

	// Add specular component
	final += lightIntensity * getSpecularFact(
		normal, lightDir.xyz, worldPos.xyz, specularIntensity, shininess
	);

	f_color = vec4(final, 1);
}

