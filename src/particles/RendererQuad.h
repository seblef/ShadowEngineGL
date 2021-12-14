#pragma once

#include "ParticleRenderer.h"
#include "../mediacommon/IIndexBuffer.h"
#include "../mediacommon/IVideoDevice.h"

namespace Particles
{

class RendererQuad : public ParticleRenderer
{
	protected:

		std::unique_ptr<Core::VxBase[]> _vertices;
		ITexture* _lastTexture;
		std::unique_ptr<IIndexBuffer> _indexBuffer;
		BlendState _lastBlend;

	public:

		RendererQuad(IVideoDevice* d);

		const char* getRendererName() const { return "quad"; }

		void addParticle(Particle& p, const Core::Camera& c);
		void renderParticles();
};

}