
#version 420 core
#include "SceneInfos.glsl"

#define USE_NORMALS

// Gather defines
#define RING_1    (1)
#define RING_2    (2)
#define RING_3    (3)
#define RING_4    (4)
#define NUM_RING_1_GATHERS    (2)
#define NUM_RING_2_GATHERS    (6)
#define NUM_RING_3_GATHERS    (12)
#define NUM_RING_4_GATHERS    (20)


#define RING_COUNT		2
#define RING_GATHER_COUNT	6

#define NUM_NORMAL_LOADS (4)
ivec2 _normalLoadPattern[NUM_NORMAL_LOADS] =
{
	ivec2( 1, 8),
	ivec2( 8,-1),
	ivec2( 5, 4),
	ivec2( 4,-4)
};

// Ring sample pattern
vec2 _HDAORingPattern[NUM_RING_4_GATHERS] =
{
	// Ring 1
	vec2( 1,-1),
	vec2( 0, 1),

	// Ring 2
	vec2( 0, 3),
	vec2( 2, 1),
	vec2( 3,-1),
	vec2( 1,-3),

	// Ring 3
	vec2( 1,-5),
	vec2( 3,-3),
	vec2( 5,-1),
	vec2( 4, 1),
	vec2( 2, 3),
	vec2( 0, 5),

	// Ring 4
	vec2( 0, 7),
	vec2( 2, 5),
	vec2( 4, 3),
	vec2( 6, 1),
	vec2( 7,-1),
	vec2( 5,-3),
	vec2( 3,-5),
	vec2( 1,-7)
};

// Ring weights
vec4 _HDAORingWeight[NUM_RING_4_GATHERS] =
{
	// Ring 1 (Sum = 5.30864)
	vec4(1.00000, 0.50000, 0.44721, 0.70711),
	vec4(0.50000, 0.44721, 0.70711, 1.00000),

	// Ring 2 (Sum = 6.08746)
	vec4(0.30000, 0.29104, 0.37947, 0.40000),
	vec4(0.42426, 0.33282, 0.37947, 0.53666),
	vec4(0.40000, 0.30000, 0.29104, 0.37947),
	vec4(0.53666, 0.42426, 0.33282, 0.37947),

	// Ring 3 (Sum = 6.53067)
	vec4(0.31530, 0.29069, 0.24140, 0.25495),
	vec4(0.36056, 0.29069, 0.26000, 0.30641),
	vec4(0.26000, 0.21667, 0.21372, 0.25495),
	vec4(0.29069, 0.24140, 0.25495, 0.31530),
	vec4(0.29069, 0.26000, 0.30641, 0.36056),
	vec4(0.21667, 0.21372, 0.25495, 0.26000),

	// Ring 4 (Sum = 7.00962)
	vec4(0.17500, 0.17365, 0.19799, 0.20000),
	vec4(0.22136, 0.20870, 0.24010, 0.25997),
	vec4(0.24749, 0.21864, 0.24010, 0.28000),
	vec4(0.22136, 0.19230, 0.19799, 0.23016),
	vec4(0.20000, 0.17500, 0.17365, 0.19799),
	vec4(0.25997, 0.22136, 0.20870, 0.24010),
	vec4(0.28000, 0.24749, 0.21864, 0.24010),
	vec4(0.23016, 0.22136, 0.19230, 0.19799)
};

float _ringWeightsTotal[RING_4] =
{
	5.30864,
	11.39610,
	17.92677,
	24.93639,
};

layout(std140, binding=2)	uniform		HDAOBuffer
{
	vec2			_renderTargetSize;
	float			_acceptAngle;
	float			_q;
	float			_qTimesNear;
	float			_normalScale;
	float			_HDAOAcceptRadius;
	float			_HDAORejectRadius;
	float			_HDAOIntensity;
	vec3			_HDAOPack;
};

in	vec2			v_uv;

out	vec4			f_color;

layout(binding=0)		uniform sampler2D	tNormal;
layout(binding=1)		uniform sampler2D	tDepth;

void	gatherZSamples(in vec2 tc, out vec4 zg)
{
	zg.x=textureOffset( tDepth, tc, ivec2(0,1)).r;
	zg.y=textureOffset( tDepth, tc, ivec2(1,1)).r;
	zg.z=textureOffset( tDepth, tc, ivec2(1,0)).r;
	zg.w=textureOffset( tDepth, tc, ivec2(0,0)).r;

	zg=-_qTimesNear / (zg - _q);
}

void	normalRejectionTest(in ivec2 sc, in ivec2 screenSize, out float ret)
{
	ivec2	mirrorPattern;
	ivec2	offsetScreenCoord;
	ivec2	mirrorOffsetScreenCoord;
	vec3	n1, n2;
	float	dotVal;
	float	sumDot = 0.0f;

	for(int normal = 0; normal<NUM_NORMAL_LOADS; ++normal)
	{
		mirrorPattern = (_normalLoadPattern[normal] + ivec2(1, 1)) * ivec2(-1, -1);
		offsetScreenCoord = sc + _normalLoadPattern[normal];
		mirrorOffsetScreenCoord = sc + mirrorPattern;

		offsetScreenCoord = clamp(offsetScreenCoord, ivec2(0,0), screenSize);
		mirrorOffsetScreenCoord = clamp(mirrorOffsetScreenCoord, ivec2(0,0), screenSize);

		n1 = texelFetch(tNormal, offsetScreenCoord, 0).xyz * 2.0f - 1.0f;
		n2 = texelFetch(tNormal, mirrorOffsetScreenCoord, 0).xyz * 2.0f - 1.0f;

		dotVal = dot(n1, n2);
		sumDot += (dotVal > _acceptAngle) ? 0.0f : (1.0f - (abs(dotVal) * 0.25f));
	}

	ret=(0.5f + sumDot * 0.25f);
}

void	gatherNormals(in vec2 tc, out vec4 g)
{
	vec3 n;

	n = textureOffset( tNormal, tc, ivec2(0,1)).xyz * 2.0f -1.0f;
	n = normalize(mat3(view)* n);
	g.x = n.z;

	n = textureOffset( tNormal, tc, ivec2(1,1)).xyz * 2.0f -1.0f;
	n = normalize(mat3(view)* n);
	g.y = n.z;

	n = textureOffset( tNormal, tc, ivec2(1,0)).xyz * 2.0f -1.0f;
	n = normalize(mat3(view)* n);
	g.z = n.z;

	n = textureOffset( tNormal, tc, ivec2(0,0)).xyz * 2.0f -1.0f;
	n = normalize(mat3(view)* n);
	g.w = n.z;
}


void main(void)
{
	vec2	kernelScale = vec2(_renderTargetSize.x / 1024.0f, _renderTargetSize.y / 768.0f);
	ivec2	screenCoord = ivec2(v_uv * _renderTargetSize);
	ivec2	screenSize = ivec2(_renderTargetSize)-ivec2(1, 1);
	vec2	fscreenCoord;
	vec2	texCoord;
	vec3	centerNormal;
	float	depth, centerZ, centerNormalZ, offsetCenterZ;
	vec2	mirrorScreenCoord, mirrorTexCoord;
	vec4	sampledZ[2];
	vec4	diff,compare[2];
	vec4	occlusion = vec4(0,0,0,0);
	vec4	sampledNormalZ[2], offsetSampledZ[2];
	float	retOcclusion, dotVal;
		
	normalRejectionTest(screenCoord,screenSize, dotVal);

	if (dotVal > 0.5f)
	{
		fscreenCoord = vec2(screenCoord);
		texCoord = fscreenCoord * invScreenSize;

		depth = texture(tDepth, texCoord).x;
		centerZ = -_qTimesNear / (depth - _q);

//		return float4(depth, 0, 0, 1);

#ifdef USE_NORMALS
		//		Check here if normal must be in world or camera space
		centerNormal = texture( tNormal, texCoord).xyz * 2.0f - 1.0f;
		centerNormal = normalize(mat3(view) * centerNormal);
		centerNormalZ = centerNormal.z;
		offsetCenterZ = centerZ + centerNormalZ * _normalScale;
#endif

		for (int g = 0; g < RING_GATHER_COUNT; ++g)
		{
			mirrorScreenCoord = ((kernelScale * _HDAORingPattern[g]) + vec2(1.0f, 1.0f)) * vec2(-1.0f, -1.0f);
			texCoord = vec2((fscreenCoord + (kernelScale * _HDAORingPattern[g])) * invScreenSize);
			mirrorTexCoord = vec2((fscreenCoord + mirrorScreenCoord) * invScreenSize);

			gatherZSamples(texCoord, sampledZ[0]);
			gatherZSamples(mirrorTexCoord, sampledZ[1]);

			diff = centerZ.xxxx - sampledZ[0];
			for(int i=0;i<4;++i)
			{
				compare[0][i] = diff[i] < _HDAORejectRadius ? 1.0f : 0.0f;
				compare[0][i] *= diff[i] > _HDAOAcceptRadius ? 1.0f : 0.0f;
			}

			diff = centerZ.xxxx - sampledZ[1];
			for(int i=0;i<4;++i)
			{
				compare[1][i] = diff[i] < _HDAORejectRadius ? 1.0f : 0.0f;
				compare[1][i] *= diff[i] > _HDAOAcceptRadius ? 1.0f : 0.0f;
			}

			occlusion.xyzw += (_HDAORingWeight[g].xyzw * (compare[0].xyzw * compare[1].zwxy) * dotVal);

#ifdef USE_NORMALS
//			sampledNormalZ[0] = tNormal.GatherBlue(sPoint, texCoord) * 2.0f - 1.0f;
//			sampledNormalZ[1] = tNormal.GatherBlue(sPoint, mirrorTexCoord) * 2.0f - 1.0f;

			gatherNormals(texCoord, sampledNormalZ[0]);
			gatherNormals(mirrorTexCoord, sampledNormalZ[1]);

			offsetSampledZ[0] = sampledZ[0] + (sampledNormalZ[0] * _normalScale);
			offsetSampledZ[1] = sampledZ[1] + (sampledNormalZ[1] * _normalScale);

			diff = offsetCenterZ.xxxx - offsetSampledZ[0];
			for(int i=0;i<4;++i)
			{
				compare[0][i] = diff[i] < _HDAORejectRadius ? 1.0f : 0.0f;
				compare[0][i] *= diff[i] > _HDAOAcceptRadius ? 1.0f : 0.0f;
			}

			diff = offsetCenterZ.xxxx - offsetSampledZ[1];
			for(int i=0;i<4;++i)
			{
				compare[1][i] = diff[i] < _HDAORejectRadius ? 1.0f : 0.0f;
				compare[1][i] *= diff[i] > _HDAOAcceptRadius ? 1.0f : 0.0f;
			}

			occlusion.xyzw += (_HDAORingWeight[g].xyzw * (compare[0].xyzw * compare[1].zwxy) * dotVal);
#endif
		}
	}

#ifdef USE_NORMALS
	retOcclusion = ((occlusion.x + occlusion.y + occlusion.z + occlusion.w) / (3.0f * _ringWeightsTotal[RING_COUNT - 1]));
#else
	retOcclusion = ((occlusion.x + occlusion.y + occlusion.z + occlusion.w) / (2.0f * _ringWeightsTotal[RING_COUNT - 1]));
#endif

	retOcclusion *= _HDAOIntensity;
	retOcclusion = 1.0f - clamp(retOcclusion,0.0f,1.0f);

	f_color=retOcclusion.xxxx;
}
