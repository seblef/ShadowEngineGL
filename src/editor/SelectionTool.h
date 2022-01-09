#pragma once

#include "ITool.h"

namespace Editor
{

class SelectionTool : public ITool
{
public:
    SelectionTool(ToolType type);

    void begin(
        int mouseX,
        int mouseY,
        int wheel,
        unsigned int flags
    );
    void onMouseButtonPressed(MouseButton button);
};

class SelectTool : public SelectionTool
{
public:
    SelectTool();

    void begin(
        int mouseX,
        int mouseY,
        int wheel,
        unsigned int flags
    );
};

}