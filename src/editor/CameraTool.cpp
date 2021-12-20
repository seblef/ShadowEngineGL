#include "CameraTool.h"
#include "EdCamera.h"
#include "../loguru.hpp"

namespace Editor
{

const float TranslationFactor = 0.1f;

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
    if(_buttonPressed[MB_LEFT])
    {
        Core::Vector3 trans = _camera->getCamera().getXAxis() * (float)-deltaX * TranslationFactor;
        Core::Vector3 zAxis(_camera->getCamera().getXAxis() ^_camera->getCamera().getYAxis());
        trans += zAxis * TranslationFactor * (float)deltaY;
        _camera->translate(Core::Vector2(trans.x, trans.z));
    }
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
