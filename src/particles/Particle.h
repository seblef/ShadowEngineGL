#pragma once

#include "Material.h"
#include "../core/Vector3.h"
#include "../core/Color.h"


namespace Particles
{

class ParticleRenderer;

class Particle
{
	public:
		float _life;
		float _mass;
		float _friction;
		float _percent, _percentCounter;
		
		Core::Vector3 _pos;
		Core::Vector3 _vel;
		Core::Color _color, _colorCounter;
		Core::Vector3 _size, _sizeCounter;

		float				_distFromCam;

		Material*	_material;
		ParticleRenderer*	_renderer;
};

}