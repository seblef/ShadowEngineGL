#include "SelectionTool.h"
#include "EdCamera.h"
#include "EditorSystem.h"
#include "Object.h"
#include "Selection.h"

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
    Core::Vector3 origin, dir;
    EditorSystem::getSingletonRef().getCamera().getCamera().makeRayFrom2D(
        _lastX,
        _lastY,
        origin,
        dir
    );

    Core::Camera& cam(EditorSystem::getSingletonRef().getCamera().getCamera());
	bool g;
    Object* obj = Selection::getSingletonRef().select(
        origin,
        dir,
        false,
        g
    );
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