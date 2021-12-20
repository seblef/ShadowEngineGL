#pragma once

#include "ITool.h"

class ITexture;
class IVideoDevice;

namespace Editor
{

class ToolWindow
{
protected:
    ITexture* _toolsTexture;
    ToolType _currentTool;

    void drawToolButton(ToolType type);

public:
    ToolWindow(IVideoDevice* device);
    ~ToolWindow();

    void setCurrentTool(ToolType tool) { _currentTool = tool; }

    void draw();
};

}