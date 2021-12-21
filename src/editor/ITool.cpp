#include "ITool.h"
#include "EditorSystem.h"
#include "../core/Plane.h"


namespace Editor
{

const Core::Plane GroundPlane(Core::Vector3(.0f, .0f, .0f), Core::Vector3(.0f, 1.f, .0f));

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

bool ITool::getGroundPosition(
    int screenX,
    int screenY,
    Core::Vector3& pos
) const
{
    Core::Vector3 origin, dir;
    EditorSystem::getSingletonRef().getCamera().getCamera().makeRayFrom2D(
        screenX, screenY,
        origin, dir
    );
    float t;
    if(!GroundPlane.intersectLine(origin, dir, t))
        return false;
    else if(t > .0f)
    {
        pos = origin + dir * t;
        return true;
    }
    else
        return false;
}

}