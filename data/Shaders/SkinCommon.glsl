
#ifndef _SKINCOMMON_H_
#define _SKINCOMMON_H_

layout(std140, binding=4)	uniform		BonesInfos
{
	mat4		boneMat[64];
};

vec4 getLocalPosition()
{
	vec4 wp = worldMat * vec4(pos,1);
	vec4 v1 = boneMat[boneIndices[0]] * wp;
	vec4 v2 = boneMat[boneIndices[1]] * wp;
	vec4 v3 = boneMat[boneIndices[2]] * wp;
	
	v1 *= boneWeights[0];
	v2 *= boneWeights[1];
	v3 *= boneWeights[2];

	return v1+v2+v3;
}

vec3 getLocalNormalBinormalTangent(vec3 src, mat3 rot)
{
	vec3 wp = rot * src;
	vec3 v1 = mat3(boneMat[boneIndices[0]]) * wp;
	vec3 v2 = mat3(boneMat[boneIndices[1]]) * wp;
	vec3 v3 = mat3(boneMat[boneIndices[2]]) * wp;

	v1 *= boneWeights[0];
	v2 *= boneWeights[1];
	v3 *= boneWeights[2];
	
	return normalize(v1 + v2 + v3);
}

#endif
