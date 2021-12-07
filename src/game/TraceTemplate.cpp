
#include "TraceTemplate.h"
#include "../core/Vector2.h"
#include "../core/Color.h"
#include "../core/YAMLCore.h"
#include "../particles/ParticleSystemTemplate.h"


TraceTemplate::TraceTemplate(const YAML::Node& node)
{
	_density = node["density"].as<float>();
    YAML::Node particles = node["particle_system"];
	_particleSystem = new ParticleSystemTemplate(particles);
}

TraceTemplate::~TraceTemplate()
{
	if (_particleSystem)
		delete _particleSystem;
}

