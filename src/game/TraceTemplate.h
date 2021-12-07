
#pragma once

class ParticleSystemTemplate;

namespace YAML
{
	class Node;
}

class TraceTemplate
{
protected:

	ParticleSystemTemplate*			_particleSystem;
	float							_density;

public:

	TraceTemplate(const YAML::Node& node);
	TraceTemplate(ParticleSystemTemplate* ps, float density) :
		_particleSystem(ps), _density(density)						{}
	~TraceTemplate();

	ParticleSystemTemplate*			getParticleSystem() const		{ return _particleSystem; }
	float							getDensity() const				{ return _density;  }
};
