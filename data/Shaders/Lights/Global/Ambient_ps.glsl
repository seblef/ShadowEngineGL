
#version 420 core
#include "SceneInfos.glsl"
#include "Lights/LightCommon.glsl"

layout(std140, binding=2)	uniform		LightInfos
{
	vec4		lightColor;
	vec4		lightDir;
	vec4		ambientLight;
};

in vec2		v_uv;

out vec3	f_color;

void main(void)
{
	vec4 acc=texture( tAccumulation, v_uv);

	vec3 spec;
	getSpecularColor(acc.rgb,acc.a,spec);

	vec3 light=ambientLight.rgb + acc.rgb;
	vec3 aldebo= texture( tAldebo, v_uv).rgb;

	float occlusion=texture( tHDAO, v_uv).r;

	f_color=aldebo * light * occlusion;	
}

