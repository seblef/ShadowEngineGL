
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

out vec4	f_color;

void main(void)
{
	vec4 accumulationData = texture(tAccumulation, v_uv);
	vec3 accumulatedDiffuse = accumulationData.rgb;

	vec4 colorData = texture(tAldebo, v_uv);
	vec3 aldebo = colorData.rgb;

	vec3 final = (ambientLight.rgb + accumulatedDiffuse) * aldebo;
	final += getSpecularColor(accumulatedDiffuse, accumulationData.a);

	f_color = vec4(final, 1);	
}
