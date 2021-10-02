#pragma once

#include "ParticleEmitter.h"

class ParticleLinearEmitter
{
public:

	ParticleLinearEmitter()			{}
	
	static void			emit(ParticleEmitter& emitter, const Vector3& p1,
		const Vector3& p2, unsigned int count);
};
