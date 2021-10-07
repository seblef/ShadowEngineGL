
#version 420 core
#include "SceneInfos.glsl"
#include "Common.glsl"

in	vec4	v_worldPos;
in	vec3	v_normal;
in	vec2	v_texBase;
in	mat3	v_tangent;

out	vec4	f_color[2];

void main(void)
{
	vec4 col, spec, tex;
	vec3 normal;
	float shine;

	col=texture( tDiffuse, v_texBase) * matDiffuse;
	tex=texture( tSpecular, v_texBase);
	spec.rgb=matSpecular.rgb * tex.rgb;

	shine=(tex.a * 255.0f + matShininess.x) * 0.5f;
	
	normal=normalize((texture( tNormal, v_texBase).rgb - 0.5f) * 2.0f);
	normal= normalize(v_tangent * normal);

	f_color[0]=vec4(col.rgb, shine / 255.0f);
	f_color[1]=vec4(normal* 0.5f + 0.5f, matShininess.y * 0.1f);
}

