#include "CameraTool.h"
#include "EdCamera.h"
#include "../loguru.hpp"

namespace Editor
{

CameraTool::CameraTool(
    EdCamera* camera,
    int mouseX,
    int mouseY,
    int mouseWheel
) :
    ITool(
        TOOL_CAMERA,
        mouseX,
        mouseY,
        mouseWheel
    ),
    _camera(camera)
{
}

void CameraTool::onMouseMove(int deltaX, int deltaY)
{
    ITool::onMouseMove(deltaX, deltaY);

    if(_buttonPressed[MB_RIGHT])
        _camera->rotate(Core::Vector2((float)deltaY, (float)deltaX));
    // if(_buttonPressed[MB_LEFT])
}

void CameraTool::onMouseWheel(int wheel)
{
    if(wheel < 0)
        _camera->scaleDistance(.9f);
    else if(wheel > 0)
        _camera->scaleDistance(1.1f);
    ITool::onMouseWheel(wheel);
}

}
