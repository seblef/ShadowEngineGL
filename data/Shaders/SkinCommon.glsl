
#ifndef _SKINCOMMON_H_
#define _SKINCOMMON_H_

layout(std140, binding=4)	uniform		BonesInfos
{
	mat4		boneMat[64];
};

void	getLocalPosition(out vec4 lp)
{
	vec4	wp, v1,v2,v3;
	wp=worldMat * vec4(pos,1);
	v1=boneMat[boneIndices[0]] * wp;
	v2=boneMat[boneIndices[1]] * wp;
	v3=boneMat[boneIndices[2]] * wp;
	
	v1*=boneWeights[0];
	v2*=boneWeights[1];
	v3*=boneWeights[2];

	lp=v1+v2+v3;
}

void	getLocalNormalBinormalTangent(in vec3 src, in mat3 rot, out vec3 dst)
{
	vec3 wp, v1,v2,v3;

	wp=rot * src;
	v1=mat3(boneMat[boneIndices[0]]) * wp;
	v2=mat3(boneMat[boneIndices[1]]) * wp;
	v3=mat3(boneMat[boneIndices[2]]) * wp;

	v1*=boneWeights[0];
	v2*=boneWeights[1];
	v3*=boneWeights[2];
	
	dst=normalize(v1+v2+v3);
}


#endif
