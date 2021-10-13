
#version 420 core
#include "SceneInfos.glsl"
#include "Lights/LightCommon.glsl"
#include "Lights/LightInfos.glsl"
#include "Lights/ShadowCommon.glsl"

out	vec4	f_color;

void main(void)
{
	vec2 uv = (gl_FragCoord.xy - vec2(0.5f,0.5f)) * invScreenSize;
	float depth = texture(tDepth, uv).r;

	vec4 point = getWorldPosition(vec3(uv, depth));
	vec3 pointToLight = lightPos.xyz - point.xyz;
	float dist = length(pointToLight);
	pointToLight /= dist;
	
	// Early exit if point is out of projector
	float DdotL = -dot(lightDir.xyz, pointToLight);
	if(DdotL < farAngle || dist > lightRange)
		discard;

	vec4 normalData = texture(tNormal, uv);
	vec3 normal = normalData.xyz * 2.0f - 1.0f;

	float NdotL = dot(normal, pointToLight);
	// Early exit if point isn't facing the light
	if(NdotL < 0.0f)
		discard;

	float shadowAttenuation = getShadowFactPoisson(point);
	// Early exit if point is completely in the shadows
	if(shadowAttenuation < 0.001)
		discard;

	// Projector attenuation
	float attenuation = clamp((DdotL - farAngle) / (nearAngle - farAngle),0.0f,1.0f);
	// Add shadow attenuation
	attenuation *= shadowAttenuation;
	// Add range attenuation
	attenuation *= getRangeAttenuation(dist);

	vec3 lightIntensity = attenuation * lightColor.rgb * lightColor.a;

	vec3 diffuse = lightIntensity * NdotL;

	float shininess = normalData.a * 255.f;
	vec3 specular = getSpecularFact(normal, pointToLight, point.xyz, 1.f, shininess) * lightIntensity;
	float specLuminance = dot(luminanceVector, specular);

	f_color = vec4(diffuse, specLuminance);
}

