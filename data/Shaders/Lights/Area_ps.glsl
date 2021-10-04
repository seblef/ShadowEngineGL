#version 420 core
#include "SceneInfos.glsl"
#include "Lights/LightCommon.glsl"
#include "Lights/LightInfos.glsl"

out	vec4	f_color;

void main(void)
{
	float depth, dist, dotDirPointToLight, diffuseFact;
	float spec, specInt, atten, specVal, specLum;
	float finalFact=1.0f;
	vec2 uv_coord;
	vec3 pointToLight, f_norm, diffuse;
	vec4 f_pos, normalFull, posInLightSpace, areaFact;

	uv_coord=(gl_FragCoord.xy - vec2(0.5f,0.5f)) * invScreenSize;
	uv_coord.y = 1.0f - uv_coord.y;
	depth=texture( tDepth, uv_coord ).r;

	getWorldPosition(vec3(uv_coord,depth),f_pos);

	pointToLight=lightPos.xyz - f_pos.xyz;
	dist=length(pointToLight);
	pointToLight/=dist;

	posInLightSpace=areaViewProj * f_pos;
	posInLightSpace.xyz/=posInLightSpace.w;

	if(posInLightSpace.x < -1.0f || posInLightSpace.x > 1.0f ||
		posInLightSpace.y < -1.0f || posInLightSpace.y > 1.0f || 
		posInLightSpace.z < 0.0f || posInLightSpace.z > 1.0f)
		discard;

	// x=left border; y=bottom border;	z=right border;	w=top border
	areaFact.xy=posInLightSpace.xy + 1.0f;
	areaFact.zw=1.0f - posInLightSpace.xy;
	areaFact=clamp(areaFact * nearAngle, 0.0f, 1.0f);
	finalFact=areaFact.x * areaFact.y * areaFact.z * areaFact.w;

	normalFull=texture( tNormal, uv_coord);
	f_norm=normalFull.xyz * 2.0f - 1.0f;

	diffuseFact=dot(f_norm,pointToLight);
	if(diffuseFact < 0.0f)
		discard;

	specInt=normalFull.a * 10.0f;
	spec=normalFull.a * 255.0f;

	getRangeFact(dist,atten);
	atten*=finalFact;
	getSpecularFact(f_norm, pointToLight, f_pos.xyz, specInt, spec, specVal);

	diffuse=lightColor.rgb * diffuseFact * atten * lightColor.a;
	getLuminance(lightColor.rgb * specVal * lightColor.a * finalFact, specLum);

	f_color=vec4(diffuse, specLum);
}
