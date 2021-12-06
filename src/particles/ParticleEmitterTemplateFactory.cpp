
#include "ParticleEmitterTemplateFactory.h"
#include "ParticlePointEmitterTemplate.h"


ParticleEmitterTemplate* ParticleEmitterTemplateFactory::createTemplate(const string& name, const YAML::Node& node)
{
	return new ParticlePointEmitterTemplate(node);
}

ParticleEmitterTemplate* ParticleEmitterTemplateFactory::createTemplate(const string& tName, const string& renderer,
	ParticleMaterial* mat, const ParticleEmitterParams& p1, const ParticleEmitterParams& p2,
	unsigned int maxParticles, const Vector3& gravity, float emissionRate)
{
	return new ParticlePointEmitterTemplate(renderer, mat, p1, p2, maxParticles, gravity, emissionRate);
}
