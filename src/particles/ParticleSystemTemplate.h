#pragma once

#include "ParticleEmitterTemplate.h"

class ParticleSystemTemplate
{
protected:

	typedef vector<ParticleEmitterTemplate*> EmitterVector;
	EmitterVector				_emitters;

	void						parse(const YAML::Node& node);

public:

	ParticleSystemTemplate(const YAML::Node& node);
	ParticleSystemTemplate(const string& psFile);
	ParticleSystemTemplate(int emitterCount, ParticleEmitterTemplate** emitters);
	~ParticleSystemTemplate();

	unsigned int				getEmitterCount() const				{ return _emitters.size(); }
	ParticleEmitterTemplate*	getEmitter(int n) const				{ return _emitters[n]; }
};
