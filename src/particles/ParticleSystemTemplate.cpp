
#include "ParticleSystemTemplate.h"
#include "ParticleEmitterTemplateFactory.h"
#include "../core/YAMLCore.h"
#include "../loguru.hpp"


ParticleSystemTemplate::ParticleSystemTemplate(const YAML::Node& node)
{
	parse(node);
}

ParticleSystemTemplate::ParticleSystemTemplate(const string& psFile)
{
	YAML::Node node;

	try
	{
		node = YAML::LoadFile(psFile);
	}
	catch(const std::exception& e)
	{
		LOG_S(ERROR) <<  e.what();
		return;
	}
	parse(node);
}

ParticleSystemTemplate::ParticleSystemTemplate(int emitterCount, ParticleEmitterTemplate** emitters)
{
	for (int i = 0; i < emitterCount; ++i)
		_emitters.push_back(emitters[i]);
}

ParticleSystemTemplate::~ParticleSystemTemplate()
{
	for(auto& e : _emitters)
		delete e;
}

void ParticleSystemTemplate::parse(const YAML::Node& node)
{
	YAML::Node emitters = node["emitters"];
	for(YAML::const_iterator e=emitters.begin(); e!= emitters.end(); ++e)
		_emitters.push_back(ParticleEmitterTemplateFactory::createTemplate((*e)["type"].as<std::string>(), *e));
}
