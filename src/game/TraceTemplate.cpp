
#include "TraceTemplate.h"
#include "../core/Vector2.h"
#include "../core/Color.h"
#include "../core/YAMLCore.h"
#include "../particles/System.h"


TraceTemplate::TraceTemplate(const YAML::Node& node)
{
	_density = node["density"].as<float>();
    YAML::Node particles = node["particle_system"];
	_particleSystem = std::unique_ptr<Particles::SystemTemplate>(new Particles::SystemTemplate(particles));
}

TraceTemplate::TraceTemplate(
    Particles::SystemTemplate* ps,
    float density
) :
	_particleSystem(ps),
    _density(density)
{
}

TraceTemplate::~TraceTemplate()
{
}
