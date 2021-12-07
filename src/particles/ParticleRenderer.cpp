
#include "ParticleRenderer.h"
#include "../mediacommon/IVertexBuffer.h"


ParticleRenderer::~ParticleRenderer()
{
	if(_vBuffer)
		delete _vBuffer;
}
