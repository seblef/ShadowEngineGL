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
	pointToLight/=dist;

	vec4 posInLightSpace = areaViewProj * point;
	posInLightSpace.xyz /= posInLightSpace.w;

	// If point is out of projected light, exit
	if(posInLightSpace.x < -1.0f || posInLightSpace.x > 1.0f ||
		posInLightSpace.y < -1.0f || posInLightSpace.y > 1.0f || 
		posInLightSpace.z < 0.0f || posInLightSpace.z > 1.0f)
		discard;

	vec4 normalData = texture(tNormal, uv);
	vec3 normal = normalData.xyz * 2.0f - 1.0f;

	// Early exit if point isn't facing the light
	float NdotL = dot(normal, pointToLight);
	if(NdotL < 0.0f)
		discard;

	// Compute attenuation at projector's borders
	// x=left border; y=bottom border;	z=right border;	w=top border
	vec4 area;
	area.xy = posInLightSpace.xy + 1.0f;
	area.zw = 1.0f - posInLightSpace.xy;
	area = clamp(area * nearAngle, 0.0f, 1.0f);
	float projectorAttenuation = area.x * area.y * area.z * area.w;

	vec3 lightIntensity =
		projectorAttenuation *
		getRangeAttenuation(dist) *
		lightColor.rgb *
		lightColor.a;

	vec3 diffuse = lightIntensity * NdotL;

	float shininess = normalData.a * 255.f;
	vec3 specular = getSpecularFact(normal, pointToLight, point.xyz, 1.f, shininess) * lightIntensity;
	float specLuminance = dot(luminanceVector, specular);

	f_color = vec4(diffuse, specLuminance);
}
