#include "NavigationPanel.h"
#include "EdMaterial.h"
#include "EdParticles.h"
#include "MaterialWindow.h"
#include "ParticlesWindow.h"
#include "Resources.h"
#include "imgui/imgui.h"


namespace Editor
{

NavigationPanel::NavigationPanel() :
    _open(true)
{
    for(int i=0;i<RES_COUNT;++i)
        _selected[i] = 0;
}

NavigationPanel::~NavigationPanel()
{
}

void NavigationPanel::draw()
{
    if(!_open)
        return;

    if(!ImGui::Begin("Navigation", &_open))
    {
        ImGui::End();
        return;
    }

    for(unsigned int i=0; i<RES_COUNT; ++i)
        drawResourceType(i);

    ImGui::End();
}

void NavigationPanel::drawResourceType(unsigned int type)
{
    const auto& resources = Resources::getSingletonRef().getAll((ResourceType)type);
    if(ImGui::TreeNode(g_ResourceNames[type]))
    {
        for(auto const& r : resources)
        {
            IResource *res = r.second.get();
            bool selected = (_selected[type] == res);
            if(ImGui::Selectable(r.second->getName().c_str(), &selected, ImGuiSelectableFlags_AllowDoubleClick))
                if(ImGui::IsMouseDoubleClicked(0))
                {
                    _selected[type] = res;
                    openResourceWindow((ResourceType)type, res);
                }
        }
        ImGui::TreePop();
    }
}

void NavigationPanel::openResourceWindow(ResourceType type, IResource* res)
{
    if(!res || res->isEdited())
        return;
    
    switch (type)
    {
    case RES_MATERIAL:
        {
            EdMaterial* mat = (EdMaterial*)res;
            new MaterialWindow(mat);
        }
        break;
    
    case RES_PARTICLES:
        {
            EdParticles* particles = (EdParticles*)res;
            new ParticlesWindow(particles);
        }
        break;
    
    default:
        break;
    }
}

}