#pragma once

#include <memory>

namespace Particles
{
class SystemTemplate;
}

namespace YAML
{
	class Node;
}

class TraceTemplate
{
protected:

	std::unique_ptr<Particles::SystemTemplate> _particleSystem;
	float _density;

public:

	TraceTemplate(const YAML::Node& node);
	TraceTemplate(Particles::SystemTemplate* ps, float density);
	~TraceTemplate();

	Particles::SystemTemplate* getParticleSystem() const { return _particleSystem.get(); }
	float getDensity() const { return _density;  }
};
