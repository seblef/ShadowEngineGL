#pragma once

#include "IWindow.h"
#include <chrono>
#include <memory>

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

public:
    ParticlesWindow(EdParticles* particles);
    ~ParticlesWindow();

    void draw();
};

}