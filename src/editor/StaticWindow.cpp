#include "StaticWindow.h"
#include "EditorSystem.h"
#include "EdStatic.h"
#include "PreviewMesh.h"
#include "../renderer/GeometryData.h"
#include "../renderer/Mesh.h"
#include "../renderer/Renderer.h"
#include "imgui/imgui.h"

namespace Editor
{

StaticWindow::StaticWindow(EdStaticTemplate* stat) :
    _static(stat),
    _preview(0),
    _startTime(std::chrono::steady_clock::now()),
    _solidWidgets(stat, 0),
    _changed(false)
{
    _static->backup();
    const Mesh *mesh = _static->getMesh();

    _preview = new PreviewMesh(
        mesh->getGeometry(),
        mesh->getMaterial(),
        Renderer::getSingletonRef().getVideoDevice(),
        EditorSystem::getSingletonRef().getPreviewResources(),
        256, 256
    );
    _solidWidgets.setPreview(_preview);
    stat->startEdit();
}

StaticWindow::~StaticWindow()
{
    _static->stopEdit();
    if(_preview)
        delete _preview;
}

void StaticWindow::draw()
{
    std::string winName = "Static " + _static->getName();
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize;
    if(_changed)
        flags |= ImGuiWindowFlags_UnsavedDocument;
    if(ImGui::Begin(winName.c_str(), &_open, flags))
    {
        std::chrono::duration<float> time = std::chrono::steady_clock::now() - _startTime;
        
        if(_preview)
            _preview->draw(time.count());

        _changed |= _solidWidgets.draw();

        ImGui::Separator();
        if(ImGui::Button("OK"))
            _open = false;
        ImGui::SameLine();
        if(ImGui::Button("Restore"))
        {
            _static->restore();
            _preview->setMaterial(_static->getMesh()->getMaterial());
            _preview->setGeometryData(_static->getMesh()->getGeometry());
            _changed = false;
        }
    }

    ImGui::End();
}

}