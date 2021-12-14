#pragma once

#include "ParticleRenderer.h"
#include "../mediacommon/IVideoDevice.h"

namespace Particles
{

class RendererPoint : public ParticleRenderer
{
protected:
    std::unique_ptr<Core::VxBase[]> _vertices;
    BlendState _lastBlend;
    ITexture* _whiteTexture;

public:
    RendererPoint(IVideoDevice *d);
    ~RendererPoint();

    const char* getRendererName() const { return "point"; }

    void addParticle(Particle& p, const Core::Camera& c);
    void renderParticles();

};

}
