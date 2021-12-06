
#pragma once

#include "../StdRenderer.h"
#include "TemplateMesh.h"
#include "TemplateParticleSystem.h"

class TextParser
{
protected:

	static TextureSlot				getTextureSlot(const string& s);
	static BlendMode				getBlendMode(const string& bm);
	static CullMode					getCullMode(const string& c);
	static void						parseMaterialAnimation(MaterialCreate& mc, const YAML::Node& node);

public:

	TextParser()			{}
	~TextParser()			{}

	static GameMaterial*			parseMaterial(const YAML::Node& node, const string& name);
	static TemplateMesh*			parseMesh(const YAML::Node& node, const string& name, const string* geoDir = 0);
	static TemplateParticleSystem*	parseParticles(const YAML::Node& node, const string& name);
	static void						parseLight(const YAML::Node& node, LightCreate_t& lc, Light::LightType& t);
};
