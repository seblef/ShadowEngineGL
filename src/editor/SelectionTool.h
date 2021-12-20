#pragma once

#include "ITool.h"

namespace Editor
{

class SelectionTool : public ITool
{
public:
    SelectionTool(
        int mouseX,
        int mouseY,
        int mouseWheel
    );
    ~SelectionTool();

    void onMouseButtonPressed(MouseButton button);
    void onMouseButtonReleased(MouseButton button);
    void onMouseMove(int deltaX, int deltaY);
    void onMouseWheel(int wheel);
};

}