
#version 420 core
#include "vx3d.glsl"
#include "SceneInfos.glsl"
#include "Common.glsl"

out	vec4	v_worldPos;
out	vec3	v_normal;
out	vec2	v_texBase;
out	mat3	v_tangent;


void main(void)
{
	vec3 t,b;
	mat3 rot=mat3(worldMat);

	v_worldPos=viewProj * (worldMat * vec4(pos,1));
	v_normal=normalize(rot * norm);
	
/*	v_texBase=vec2(dot(matUVRotX,uv), dot(matUVRotY,uv));
	v_texBase*=matUVScale;
	v_texBase+=matUVOffset;*/
	v_texBase=uv;

	t=normalize(rot * tangent);
	b=normalize(rot * binormal);

	v_tangent=mat3(t,b,v_normal);
/*	v_tangent[0]=vec3(t.x, b.x, v_normal.x);
	v_tangent[1]=vec3(t.y, b.y, v_normal.y);
	v_tangent[2]=vec3(t.z, b.z, v_normal.z);
*/
	gl_Position=v_worldPos;
}

