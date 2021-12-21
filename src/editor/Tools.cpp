#include "Tools.h"
#include "CameraTool.h"
#include "SelectionTool.h"
#include "WorldTransformTools.h"


namespace Editor
{

Tools::Tools(int mouseX, int mouseY, int mouseWheel) :
    _lastX(mouseX),
    _lastY(mouseY),
    _lastWheel(mouseWheel),
    _currentTool(0),
    _flags(0)
{
    _tools[TOOL_CAMERA] = new CameraTool;
    _tools[TOOL_SELECTION] = new SelectTool;
    _tools[TOOL_TRANSLATE] = new TranslationTool;

    setCurrentTool(TOOL_SELECTION);
}

Tools::~Tools()
{
    for(auto const& tool : _tools)
        delete tool.second;
}

void Tools::setCurrentTool(ToolType type)
{
    if(_currentTool)
        _currentTool->end();
    _currentTool = _tools[type];
    _currentTool->begin(
        _lastX, _lastY,
        _lastWheel, _flags
    );
}

void Tools::cancel()
{
    _currentTool->cancel();
}

void Tools::setFlag(unsigned int flag)
{
    _flags |= flag;
    _currentTool->setFlags(_flags);
}

void Tools::unsetFlag(unsigned int flag)
{
    _flags = _flags & (~flag);
    _currentTool->setFlags(_flags);
}

void Tools::onMouseButtonPressed(MouseButton button)
{
    _currentTool->onMouseButtonPressed(button);
}

void Tools::onMouseButtonReleased(MouseButton button)
{
    _currentTool->onMouseButtonReleased(button);
}

void Tools::onMouseMove(int deltaX, int deltaY)
{
    _lastX += deltaX;
    _lastY += deltaY;
    _currentTool->onMouseMove(deltaX, deltaY);
}

void Tools::onMouseWheel(int wheel)
{
    _lastWheel = wheel;
    _currentTool->onMouseWheel(wheel);
}

}