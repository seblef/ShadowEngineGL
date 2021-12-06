#pragma once

#include "ParticleEmitterTemplate.h"

class ParticlePointEmitterTemplate : public ParticleEmitterTemplate
{
public:

	ParticlePointEmitterTemplate(const YAML::Node& node) : ParticleEmitterTemplate("point", node) {}
	ParticlePointEmitterTemplate(const string& renderer,
		ParticleMaterial* mat,
		const ParticleEmitterParams& p1,
		const ParticleEmitterParams& p2,
		unsigned int maxParticles,
		const Vector3& gravity,
		float emissionRate) : ParticleEmitterTemplate("point", renderer,
		mat, p1, p2, maxParticles, gravity, emissionRate)		{}

};
