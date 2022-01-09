#include "SelectionTool.h"
#include "EdCamera.h"
#include "EditorSystem.h"
#include "Object.h"
#include "Selection.h"

namespace Editor
{

SelectionTool::SelectionTool(ToolType type) : ITool(type)
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
	bool groundSelected;
    Object* obj = Selection::getSingletonRef().select(
        origin,
        dir,
        _flags & TF_CTRLDOWN,
        groundSelected
    );
    ITool::onMouseButtonPressed(button);
}

void SelectionTool::begin(
    int mouseX,
    int mouseY,
    int wheel,
    unsigned int flags
)
{
    ITool::begin(mouseX, mouseY, wheel, flags);
}

SelectTool::SelectTool() : SelectionTool(TOOL_SELECTION)
{
}

void SelectTool::begin(
    int mouseX,
    int mouseY,
    int wheel,
    unsigned int flags
)
{
    SelectionTool::begin(mouseX, mouseY, wheel, flags);
}

}