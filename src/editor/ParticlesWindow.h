#pragma once

#include "IWindow.h"
#include <chrono>
#include <memory>

namespace Particles
{
class Material;
class SubSystem;
class SubSystemTemplate;
}

namespace Editor
{

class EdParticles;
class PreviewParticles;

class ParticlesWindow : public IWindow
{
protected:
    EdParticles* _particles;
    std::unique_ptr<PreviewParticles> _preview;
    std::chrono::time_point<std::chrono::steady_clock> _startTime;
    bool _changed;

    void drawControls();
    void drawSubSystems();
    void drawSubSystem(int i);
    void drawMaterial(Particles::Material* mat);
    void drawTexture(Particles::Material* mat);
    void drawTextureSet(Particles::Material* mat);
    void drawGlobalParams(
        Particles::SubSystemTemplate* sub,
        Particles::SubSystem* subInstance
    );
    void drawAllParticlesParams(Particles::SubSystemTemplate* sub);

    bool subSystemExists(const std::string& name) const;

public:
    ParticlesWindow(EdParticles* particles);
    ~ParticlesWindow();

    void draw();
};

}