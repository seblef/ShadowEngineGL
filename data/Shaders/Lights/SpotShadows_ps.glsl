
#version 420 core
#include "SceneInfos.glsl"
#include "Lights/LightCommon.glsl"
#include "Lights/LightInfos.glsl"
#include "Lights/ShadowCommon.glsl"

out	vec4	f_color;

void main(void)
{
	float depth, dist, dotDirPointToLight, diffuseFact, shadowFact=1;
	float spec, specInt, atten, specVal, specLum;
	float finalFact=1.0f;
	vec2 uv_coord;
	vec3 pointToLight, f_norm, diffuse;
	vec4 f_pos, normalFull;
	

	uv_coord=(gl_FragCoord.xy - vec2(0.5f,0.5f)) * invScreenSize;
	depth=texture( tDepth, uv_coord ).r;

	getWorldPosition(vec3(uv_coord,depth),f_pos);

	pointToLight=lightPos.xyz - f_pos.xyz;
	dist=length(pointToLight);
	pointToLight/=dist;
	

	dotDirPointToLight=-dot(lightDir.xyz,pointToLight);
	if(dotDirPointToLight < farAngle || dist > lightRange)
		discard;

	finalFact=clamp(( dotDirPointToLight - farAngle) / (nearAngle - farAngle),0.0f,1.0f);

	normalFull=texture( tNormal, uv_coord);
	f_norm=normalFull.xyz * 2.0f - 1.0f;

	diffuseFact=dot(f_norm,pointToLight);
	if(diffuseFact < 0.0f)
		discard;

	getShadowFactPoisson(f_pos, shadowFact);
	finalFact*=shadowFact;

	specInt=normalFull.a * 10.0f;
	spec=normalFull.a * 255.0f;

	getRangeFact(dist,atten);
	atten*=finalFact;
	getSpecularFact(f_norm, pointToLight, f_pos.xyz, specInt, spec, specVal);

	diffuse=lightColor.rgb * diffuseFact * atten * lightColor.a;
	getLuminance(lightColor.rgb * specVal * lightColor.a * finalFact, specLum);

	f_color=vec4(diffuse, specLum);
}

