#include "ToolWindow.h"
#include "Tools.h"
#include "imgui/imgui.h"
#include "../glmedia/GLTexture.h"
#include "../mediacommon/ITexture.h"
#include "../mediacommon/IVideoDevice.h"
#include <map>


namespace Editor
{

const int ButtonsPerRow = 3;
const int ButtonSize = 24;
const int IconPerRow = 8;
const Core::Vector2 IconUVSize(1.f / (float)IconPerRow, 1.f / (float)IconPerRow);

const std::map<ToolType,Core::Vector2> ToolsIconMapping = {
    { TOOL_CAMERA, Core::Vector2(0.f, 0.f) },
    { TOOL_SELECTION, Core::Vector2(1.f, .0f) },
    { TOOL_TRANSLATE, Core::Vector2(2.f, .0f) },
    { TOOL_ROTATE, Core::Vector2(3.f, .0f) }
};

const std::map<ToolType,std::string> ToolsTooltips = {
    { TOOL_CAMERA, "Camera move" },
    { TOOL_SELECTION, "Selection" },
    { TOOL_TRANSLATE, "Translation" },
    { TOOL_ROTATE, "Rotation" }
};

const std::map<unsigned int,std::string> FlagsLabels = {
    { TF_LOCKX, "Lock X axis" },
    { TF_LOCKY, "Lock Y axis" },
    { TF_LOCKZ, "Lock Z axis "},
    { TF_SNAP, "Snap" }
};


ToolWindow::ToolWindow(IVideoDevice* device) :
    _toolsTexture(0)
{
    _toolsTexture = device->createTexture("Textures/Editor/tools.png");
}

ToolWindow::~ToolWindow()
{
    if(_toolsTexture)
        _toolsTexture->remRef();
}

void ToolWindow::draw()
{
    ImGuiWindowFlags winFlags =
        ImGuiWindowFlags_AlwaysAutoResize;
    bool open = true;
    if(!ImGui::Begin("Toolbox", &open, winFlags))
    {
        ImGui::End();
        return;
    }

    for(int type=TOOL_NULL + 1; type < TOOL_COUNT; ++type)
    {
        drawToolButton((ToolType)type);
        if((type) % ButtonsPerRow != 0)
            ImGui::SameLine();
    }

    ImGui::Separator();
    drawFlags();

    ImGui::End();
}

void ToolWindow::drawToolButton(ToolType type)
{
    const Core::Vector2& iconMap(ToolsIconMapping.find(type)->second);
    const Core::Vector2 uvStart(
        iconMap.x * IconUVSize.x,
        iconMap.y * IconUVSize.y
    );
    const Core::Vector2 uvEnd(
        (iconMap.x + 1.f) * IconUVSize.x,
        (iconMap.y + 1.f) * IconUVSize.y
    );
    const std::string& toolTip(ToolsTooltips.find(type)->second);
    GLuint texID = ((GLTexture*)_toolsTexture)->getGLId();

    int framePadding = 0;
    if(type == Tools::getSingletonRef().getCurrentToolType())
        framePadding = 2;

    ImGui::PushID(type);
    if(ImGui::ImageButton(
        (ImTextureID)texID,
        ImVec2(ButtonSize, ButtonSize),
        ImVec2(uvStart.x, uvStart.y),
        ImVec2(uvEnd.x, uvEnd.y),
        framePadding,
        ImVec4(0.f, 0.f, 0.f, 1.f),
        ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
    ))
    {
        Tools::getSingletonRef().setCurrentTool(type);
    }
    ImGui::PopID();
}

void ToolWindow::drawFlags()
{
    const unsigned int flags = Tools::getSingletonRef().getFlags();
    for(auto const& flag : FlagsLabels)
    {
        bool enabled = (bool)(flags & flag.first);
        ImGui::Checkbox(flag.second.c_str(), &enabled);

        if(enabled)
            Tools::getSingletonRef().setFlag(flag.first);
        else
            Tools::getSingletonRef().unsetFlag(flag.first);
    }
}

}
