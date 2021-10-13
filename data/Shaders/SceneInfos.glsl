
#ifndef _SCENEINFOS_H_
#define _SCENEINFOS_H_

layout(std140, binding=0) 	uniform 	SceneInfos
{
	mat4				viewProj;
	mat4				invViewProj;
	mat4				invProj;
	mat4				proj;
	mat4				view;
	vec4				eyePos;
	vec2				invScreenSize;
	float				time;
	float				siPack;
};

layout(std140, binding=1)	uniform		ObjectInfos
{
	mat4				worldMat;
};

vec4 getWorldPosition(vec3 screenPos)
{
	vec4 H=vec4(screenPos*2.0f - 1.0f, 1.0f);
	vec4 world_pos = invViewProj * H;
	return world_pos / world_pos.w;
}

vec4 getViewPosition(vec3 screenPos)
{
	vec4 H=vec4(screenPos*2.0f - 1.0f, 1.0f);
	vec4 view_pos = invProj * H;
	return view_pos / view_pos.w;
}

#endif
