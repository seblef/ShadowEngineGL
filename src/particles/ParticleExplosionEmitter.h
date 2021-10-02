#pragma once

#include "ParticleEmitter.h"

class ParticleExplosionEmitter
{
public:

	ParticleExplosionEmitter() 							{}

	static void				emit(ParticleEmitter& e, float size, unsigned int count);
};
