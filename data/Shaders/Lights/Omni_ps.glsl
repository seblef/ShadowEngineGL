
#version 420 core
#include "SceneInfos.glsl"
#include "Lights/LightCommon.glsl"
#include "Lights/LightInfos.glsl"

out	vec4	f_color;

void main(void)
{
	vec2 uv_coord=(gl_FragCoord.xy - vec2(0.5f,0.5f)) * invScreenSize;
	float depth=texture( tDepth, uv_coord ).r;

	vec4 f_pos;
	getWorldPosition(vec3(uv_coord,depth),f_pos);

	vec3 pointToLight=lightPos.xyz - f_pos.xyz;
	float dist=length(pointToLight);

	pointToLight/=dist;
	if(dist > lightRange)
		discard;

	vec4 normalFull=texture( tNormal, uv_coord);
	vec3 f_normal=normalFull.xyz * 2.0f - 1.0f;

	float diffuseFact=dot(f_normal, pointToLight);
	if(diffuseFact < 0.0f)
		discard;

	float specInt=normalFull.a * 10.0f;
	float spec=normalFull.a * 255.0f;
	
	float atten, specVal;
	getRangeFact(dist,atten);

	getSpecularFact(f_normal,pointToLight,f_pos.xyz,specInt,spec,specVal);

	vec3 diffuse=lightColor.rgb * diffuseFact * atten * lightColor.a;
	float specLuminance;
	getLuminance(lightColor.rgb * specVal * lightColor.a, specLuminance);

	f_color=vec4(diffuse, specLuminance);
}
