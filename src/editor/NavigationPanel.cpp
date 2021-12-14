#include "NavigationPanel.h"
#include "EdMaterial.h"
#include "MaterialWindow.h"
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
            bool selected = (_selected[type] == r.second.get());
            if(ImGui::Selectable(r.second->getName().c_str(), &selected, ImGuiSelectableFlags_AllowDoubleClick))
                if(ImGui::IsMouseDoubleClicked(0))
                {
                    _selected[type] = r.second.get();
                    EdMaterial* mat = (EdMaterial*)r.second.get();
                    if(!mat->isEdited())
                        new MaterialWindow((EdMaterial*)r.second.get());
                }
        }
        ImGui::TreePop();
    }
}

}