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

    void drawToolButton(ToolType type);

public:
    ToolWindow(IVideoDevice* device);
    ~ToolWindow();

    void draw();
};

}