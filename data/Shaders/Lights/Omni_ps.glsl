
#version 420 core
#include "SceneInfos.glsl"
#include "Lights/LightCommon.glsl"
#include "Lights/LightInfos.glsl"

out	vec4	f_color;

void main(void)
{
	vec2 uv = gl_FragCoord.xy * invScreenSize;
	float depth = texture(tDepth, uv).r;

	vec4 point = getWorldPosition(vec3(uv, depth));

	vec3 pointToLight = lightPos.xyz - point.xyz;
	float dist = length(pointToLight);

	pointToLight /= dist;
	if(dist > lightRange)
		discard;

	vec4 normalData = texture( tNormal, uv);
	vec3 normal = normalData.xyz * 2.0f - 1.0f;

	float NdotL = dot(normal, pointToLight);
	if(NdotL < 0.0f)
		discard;

	vec3 lightIntensity = getRangeAttenuation(dist) * lightColor.rgb * lightColor.a;
	vec3 diffuse = lightIntensity * NdotL;

	float shininess = normalData.a * 255.f;
	vec3 specular = getSpecularFact(normal, pointToLight, point.xyz, 1.f, shininess) * lightIntensity;
	float specLuminance = dot(luminanceVector, specular);

	f_color = vec4(diffuse, specLuminance);
}
