#include "MaterialWindow.h"
#include "EditorSystem.h"
#include "EdMaterial.h"
#include "PreviewMesh.h"
#include "imgui/imgui.h"
#include "../renderer/Renderer.h"


namespace Editor
{

MaterialWindow::MaterialWindow(EdMaterial* material) :
    _material(material),
    _preview(0)
{
    _preview = new PreviewMesh(
        0,
        material->getMaterial(),
        Renderer::getSingletonRef().getVideoDevice(),
        EditorSystem::getSingletonRef().getPreviewResources(),
        128, 128
    );
}

MaterialWindow::~MaterialWindow()
{
    if(_preview)
        delete _preview;
}

void MaterialWindow::draw()
{
    std::string winName = "Material " + _material->getName();
    if(ImGui::Begin(winName.c_str(), &_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        // TODO: Add preview
        char buffer[64];
        strcpy(buffer, _material->getName().c_str());
        ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer));   
        
        if(_preview)
            _preview->draw();
    }

    ImGui::End();
}

}