
#include "ParticleEmitterTemplate.h"
#include "../core/YAMLCore.h"

using namespace std;

ParticleEmitterTemplate::~ParticleEmitterTemplate()
{
	if (_material)		delete _material;
}

ParticleEmitterTemplate::ParticleEmitterTemplate(const char* emitterName, const YAML::Node& node) :
	_emitterName(emitterName),
	_material(0),
	_maxParticles(10),
    _gravity(Vector3::NullVector),
	_emissionRate(0)
{
	if(node["life"])
	{
		_params1.setLife(node["life"][0].as<float>());
		_params2.setLife(node["life"][1].as<float>());
	}
	if(node["mass"])
	{
		_params1.setMass(node["mass"][0].as<float>());
		_params2.setMass(node["mass"][1].as<float>());
	}
	if(node["friction"])
	{
		_params1.setFriction(node["friction"][0].as<float>());
		_params2.setFriction(node["friction"][1].as<float>());
	}
	if(node["velocity"])
	{
		_params1.setVelocity(node["velocity"][0].as<Core::Vector3>());
		_params2.setVelocity(node["velocity"][1].as<Core::Vector3>());
	}
	if(node["start_color"])
	{
		_params1.setStartColor(node["start_color"][0].as<Core::Color>());
		_params2.setStartColor(node["start_color"][1].as<Core::Color>());
	}
	if(node["end_color"])
	{
		_params1.setEndColor(node["end_color"][0].as<Color>());
		_params2.setEndColor(node["end_color"][1].as<Color>());
	}
	if(node["start_size"])
	{
		_params1.setStartSize(node["start_size"][0].as<Core::Vector3>());
		_params2.setStartSize(node["start_size"][1].as<Core::Vector3>());
	}
	if(node["end_size"])
	{
		_params1.setEndSize(node["end_size"][0].as<Core::Vector3>());
		_params2.setEndSize(node["end_size"][1].as<Core::Vector3>());
	}
    YAML::Node material = node["material"];
	if(material)
	{
		if(_material)
			delete _material;
		_material = new Particles::Material(material);
	}

	_renderer = node["renderer"].as<string>();
	_maxParticles = node["max_particles"].as<int>(10);
	_gravity = node["gravity"].as<Vector3>(Vector3::NullVector);
	_emissionRate = node["emission_rate"].as<float>(0.f);
}
