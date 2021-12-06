#pragma once

#include "ParticleEmitterTemplate.h"

class ParticleEmitterTemplateFactory
{
public:

	ParticleEmitterTemplateFactory()			{}

	static ParticleEmitterTemplate*				createTemplate(const string& tName, const YAML::Node& node);
	static ParticleEmitterTemplate*				createTemplate(const string& tName, const string& renderer,
		ParticleMaterial* mat, const ParticleEmitterParams& p1, const ParticleEmitterParams& p2,
		unsigned int maxParticles, const Vector3& gravity, float emissionRate);
};
