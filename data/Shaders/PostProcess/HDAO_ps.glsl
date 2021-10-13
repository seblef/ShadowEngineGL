
#version 420 core
#include "SceneInfos.glsl"


#define HDAO_MAX_KERNEL_SIZE		64

const float bias = 0.025;

layout(std140, binding=2)	uniform		HDAOBuffer
{
	vec4 kernel[HDAO_MAX_KERNEL_SIZE];
	vec2 noiseScale;
	float q;
	float qTimesNear;
	float radius;
	int kernelSize;
	vec2 f2pack;
};

in	vec2			v_uv;

out	vec4			f_color;

layout(binding=1)		uniform sampler2D	tNormal;
layout(binding=2)		uniform sampler2D	tDepth;
layout(binding=3)		uniform sampler2D	tNoise;


void main(void)
{
	vec3 normal = (texture(tNormal, v_uv).xyz * 2.f) - 1.f;
	normal = mat3(view) * normal;

	float depth = texture(tDepth, v_uv).r;
	vec3 pos = getViewPosition(vec3(v_uv, depth)).xyz;

	vec3 randomVec = normalize(texture(tNoise, v_uv * noiseScale).xyz);
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.f;
	for(int i=0; i<kernelSize; ++i)
	{
		vec3 samplePos = TBN * kernel[i].xyz;
		samplePos = pos + samplePos * radius;

		vec4 offset = proj * vec4(samplePos, 1);
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5f + 0.5f;

		float sampleDepth = texture(tDepth, offset.xy).r;
		sampleDepth = getViewPosition(vec3(offset.xy, sampleDepth)).z;

		float rangeCheck = smoothstep(0.f, 1.f, radius / abs(pos.z - sampleDepth));
		occlusion += (sampleDepth <= samplePos.z - bias ? 1.f : 0.f) * rangeCheck;
	}

	occlusion = 1.f - (occlusion / kernelSize);
	f_color = vec4(occlusion, occlusion, occlusion, occlusion);
}