#pragma once

namespace Editor
{

enum ToolType
{
    TOOL_NULL=0,
    TOOL_SELECTION,
    TOOL_CAMERA,

    TOOL_COUNT
};

enum MouseButton
{
    MB_LEFT=0,
    MB_RIGHT,

    MB_COUNT
};

class ITool
{
private:
    ToolType _type;

protected:
    bool _buttonPressed[MB_COUNT];
    bool _ctrlDown;
    int _lastX, _lastY;
    int _lastWheel;

public:

    ITool(
        ToolType type,
        int mouseX,
        int mouseY,
        int mouseWheel
    );
    virtual ~ITool() {}

    virtual void onMouseButtonPressed(MouseButton button);
    virtual void onMouseButtonReleased(MouseButton button);
    virtual void onMouseMove(int deltaX, int deltaY);
    virtual void onMouseWheel(int wheel);

    void setCtrlDown(bool down) { _ctrlDown = down; }
};
}