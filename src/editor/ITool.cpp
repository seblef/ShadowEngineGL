#include "ITool.h"

namespace Editor
{

ITool::ITool(ToolType type) :
    _type(type),
    _lastX(0),
    _lastY(0),
    _lastWheel(0),
    _flags(0)
{
    for(int i=0;i<MB_COUNT;++i)
        _buttonPressed[i] = false;
}

void ITool::begin(
    int mouseX,
    int mouseY,
    int mouseWheel,
    unsigned int flags
)
{
    _lastX = mouseX;
    _lastY = mouseY;
    _lastWheel = mouseWheel;
    _flags = flags;
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