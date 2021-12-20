#include "SelectionTool.h"

namespace Editor
{

SelectionTool::SelectionTool(
    int mouseX,
    int mouseY,
    int mouseWheel
) :
    ITool(
        TOOL_SELECTION,
        mouseX,
        mouseY,
        mouseWheel
    )
{
}

SelectionTool::~SelectionTool()
{

}

void SelectionTool::onMouseButtonPressed(MouseButton button)
{
    ITool::onMouseButtonPressed(button);
}

void SelectionTool::onMouseButtonReleased(MouseButton button)
{
    ITool::onMouseButtonReleased(button);
}

void SelectionTool::onMouseMove(int deltaX, int deltaY)
{
    ITool::onMouseMove(deltaX, deltaY);
}

void SelectionTool::onMouseWheel(int wheel)
{
    ITool::onMouseWheel(wheel);
}

}