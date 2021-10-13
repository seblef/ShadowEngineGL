
#ifndef _SHADOWCOMMON_H_
#define _SHADOWCOMMON_H_

layout(std140, binding=3)	uniform		ShadowInfos
{
	mat4		lightViewProj;
	vec2		invShadowMapSize;
	float		shadowFilter;
	float		invShadowFilter;
};

vec2 poissonDisk[16]={
  vec2( -0.94201624f, -0.39906216f ),
  vec2( 0.94558609f, -0.76890725f ),
  vec2( -0.094184101f, -0.92938870f ),
  vec2( 0.34495938f, 0.29387760f ),
   vec2( -0.91588581f, 0.45771432f ), 
   vec2( -0.81544232f, -0.87912464f ), 
   vec2( -0.38277543f, 0.27676845f ), 
   vec2( 0.97484398f, 0.75648379f ), 
   vec2( 0.44323325f, -0.97511554f ), 
   vec2( 0.53742981f, -0.47373420f ), 
   vec2( -0.26496911f, -0.41893023f ), 
   vec2( 0.79197514f, 0.19090188f ), 
   vec2( -0.24188840f, 0.99706507f ), 
   vec2( -0.81409955f, 0.91437590f ), 
   vec2( 0.19984126f, 0.78641367f ), 
   vec2( 0.14383161f, -0.14100790f )
};


//			Util file for reading shadow maps

layout(binding=7)	uniform sampler2DShadow		tShadow;

float random(vec3 seed, int i)
{
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

float getShadowFactPoisson(vec4 point)
{
	float bias=0.005f;
	vec4 lightRefPos=lightViewProj * point;
	lightRefPos.z-=bias;
	lightRefPos/=lightRefPos.w;

	vec2 shadowUV=0.5f * lightRefPos.xy + vec2(0.5f,0.5f);

	float s=1.0f;

	// Sample the shadow map 4 times
	for (int i=0;i<int(shadowFilter);i++)
	{
		// use either :
		//  - Always the same samples.
		//    Gives a fixed pattern in the shadow, but no noise
		int index = i;
		//  - A random sample, based on the pixel's screen location. 
		//    No banding, but the shadow moves with the camera, which looks weird.
		// int index = int(16.0*random(gl_FragCoord.xyy, i))%16;
		//  - A random sample, based on the pixel's position in world space.
		//    The position is rounded to the millimeter to avoid too much aliasing
		// int index = int(16.0*random(floor(point.xyz*1000.0), i))%16;
		
		// being fully in the shadow will eat up 4*0.2 = 0.8
		// 0.2 potentially remain, which is quite dark.
		s -= invShadowFilter*(1.0-texture( tShadow, vec3(shadowUV + poissonDisk[index]/1000.0f , lightRefPos.z*0.5f + 0.5f)).r);
	}

	return s;
}

float getShadowFactBase(vec4 point)
{
	float bias=0.005f;
	vec4 lightRefPos=lightViewProj * point;
	lightRefPos.z-=bias;

	lightRefPos/=lightRefPos.w;

	vec2 shadowUV=0.5f * lightRefPos.xy + vec2(0.5f,0.5f);
	return texture(tShadow,vec3(shadowUV,lightRefPos.z*0.5f + 0.5f)).r;
}

vec2 getShadowUV(vec4 point)
{
	vec4 lightRefPos=lightViewProj * point;
	lightRefPos/=lightRefPos.w;

	return (0.5f * lightRefPos.xy) + vec2(0.5f,0.5f);
}

#endif
