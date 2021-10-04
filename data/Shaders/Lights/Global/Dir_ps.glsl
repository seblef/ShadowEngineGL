
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
	vec4	normalFull, color=vec4(0,0,0,0);
	vec3	normal, diffuse=vec3(0,0,0);
	float 	diff, specVal=0, shadowFact=1;
	float 	occlusion;
	vec3	lfact, ldiffuse, lspec, cfinal, cspec;
	vec4	acc;
	float 	depth, spec, specInt;
	vec4 	pos;

	normalFull=texture( tNormal, v_uv);
	normal=normalFull.xyz * 2.0f - 1.0f;

	diff=-dot(normal, lightDir.xyz);
	
	color=texture( tAldebo, v_uv);

	if(diff > 0.0f)
	{
		depth=texture( tDepth, v_uv).r;
		spec=normalFull.a * 255.0f;
		specInt=normalFull.a * 10.0f;

		getWorldPosition(vec3(v_uv,depth),pos);

		getSpecularFact(normal,-lightDir.xyz,pos.xyz,specInt,spec,specVal);
		getShadowFactPoisson(pos,shadowFact);

//		f_color=vec4(shadowFact,0,0,1);
//		return;
	}
	else
		diff=0.0f;

	occlusion=texture(tHDAO, v_uv).r;
	lfact=lightColor.rgb * lightColor.a * shadowFact;

	acc=texture(tAccumulation, v_uv);
	ldiffuse=acc.rgb + lfact * diff;

	getSpecularColor(acc.rgb, acc.a,cspec);
	lspec=lfact * specVal + cspec;

	cfinal=((ldiffuse + ambientLight.rgb) * color.rgb) * occlusion;
	f_color = vec4(cfinal, 1);
}

