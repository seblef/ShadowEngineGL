
#include "ParticleRenderer.h"

ParticleRenderer::~ParticleRenderer()
{
	if(_vBuffer)
		delete _vBuffer;
}
