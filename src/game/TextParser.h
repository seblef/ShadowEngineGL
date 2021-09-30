
#ifndef _TEXTPARSER_H_
#define _TEXTPARSER_H_

#include "../StdRenderer.h"
#include "TemplateMesh.h"
#include "TemplateParticleSystem.h"

class TextParser
{
protected:

	static TextureSlot				getTextureSlot(const string& s);
	static BlendMode				getBlendMode(const string& bm);
	static CullMode					getCullMode(const string& c);
	static void						parseVector2(ScriptFile& sf, Vector2& v);
	static void						parseMaterialAnimation(MaterialCreate& mc, ScriptFile& sf);

public:

	TextParser()			{}
	~TextParser()			{}

	static GameMaterial*			parseMaterial(ScriptFile& sf, string* matName=0);
	static TemplateMesh*			parseMesh(ScriptFile& sf, string* meshName = 0, const string* geoDir = 0);
	static TemplateParticleSystem*	parseParticles(ScriptFile& sf, string* particleName=0);
	static void						parseLight(ScriptFile& sf, LightCreate_t& lc, Light::LightType& t);
};

#endif
