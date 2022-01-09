#pragma once

#include "ITool.h"
#include "../core/TSingleton.h"
#include <map>

namespace Editor
{

class Tools : public Core::TSingleton<Tools>
{
protected:
    std::map<ToolType,ITool*> _tools;
    ITool* _currentTool;
    unsigned int _flags;
    int _lastX;
    int _lastY;
    int _lastWheel;

public:
    Tools(int mouseX, int mouseY, int mouseWheel);
    ~Tools();

    void setCurrentTool(ToolType type);
    ITool* getCurrentTool() const { return _currentTool; }
    ToolType getCurrentToolType() const { return _currentTool->getType(); }
    void cancel();

    void setFlag(unsigned int flag);
    void unsetFlag(unsigned int flag);
    unsigned int getFlags() const { return _flags; }

    void onMouseButtonPressed(MouseButton button);
    void onMouseButtonReleased(MouseButton button);
    void onMouseMove(int deltaX, int deltaY);
    void onMouseWheel(int wheel);
};

}