#include "ParticlesWindow.h"
#include "EditorSystem.h"
#include "EdParticles.h"
#include "PreviewParticles.h"
#include "imgui/imgui.h"
#include "../particles/System.h"
#include "../renderer/Renderer.h"


namespace Editor
{

ParticlesWindow::ParticlesWindow(EdParticles* particles) :
    _particles(particles),
    _startTime(std::chrono::steady_clock::now()),
    _changed(false)
{
    particles->backup();
    particles->startEdit();

    _preview = std::unique_ptr<PreviewParticles>(
        new PreviewParticles(
            &particles->getTemplate(),
            Renderer::getSingletonRef().getVideoDevice(),
            EditorSystem::getSingletonRef().getPreviewResources(),
            256, 256
        )
    );
}

ParticlesWindow::~ParticlesWindow()
{
    _particles->stopEdit();
}

void ParticlesWindow::draw()
{
    std::string winName = "Particles " + _particles->getName();
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize;
    if(_changed)
        flags |= ImGuiWindowFlags_UnsavedDocument;
    if(ImGui::Begin(winName.c_str(), &_open, flags))
    {
        std::chrono::duration<float> time = std::chrono::steady_clock::now() - _startTime;
        if(_preview)
            _preview->draw(time.count());
    }
    ImGui::End();
}

}