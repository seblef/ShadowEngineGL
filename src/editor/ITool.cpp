#include "ITool.h"

namespace Editor
{

ITool::ITool(
    ToolType type,
    int mouseX,
    int mouseY,
    int mouseWheel
) :
    _type(type),
    _lastX(mouseX),
    _lastY(mouseY),
    _lastWheel(mouseWheel),
    _ctrlDown(false)
{
    for(int i=0;i<MB_COUNT;++i)
        _buttonPressed[i] = false;
}

void ITool::onMouseButtonPressed(MouseButton button)
{
    _buttonPressed[button] = true;
}

void ITool::onMouseButtonReleased(MouseButton button)
{
    _buttonPressed[button] = false;
}

void ITool::onMouseMove(int deltaX, int deltaY)
{
    _lastX += deltaX;
    _lastY += deltaY;
}

void ITool::onMouseWheel(int wheel)
{
    _lastWheel = wheel;
}

}