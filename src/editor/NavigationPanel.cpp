#include "NavigationPanel.h"
#include "Resources.h"
#include "imgui/imgui.h"


namespace Editor
{

NavigationPanel::NavigationPanel() :
    _open(true)
{
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
    ImGuiTreeNodeFlags leaf_flags =
        ImGuiTreeNodeFlags_Leaf |
        ImGuiTreeNodeFlags_NoTreePushOnOpen |
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_OpenOnDoubleClick |
        ImGuiTreeNodeFlags_SpanAvailWidth |
        ImGuiTreeNodeFlags_Bullet;
    const auto& resources = Resources::getSingletonRef().getAll((ResourceType)type);
    if(ImGui::TreeNode(g_ResourceNames[type]))
    {
        for(auto const& r : resources)
            ImGui::TreeNodeEx(r.second->getName().c_str(), leaf_flags);
        ImGui::TreePop();
    }
}

}