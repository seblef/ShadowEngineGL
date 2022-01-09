#pragma once

namespace Core
{
class Vector3;
}

namespace Editor
{

enum ToolType
{
    TOOL_NULL=0,
    TOOL_SELECTION,
    TOOL_CAMERA,
    TOOL_TRANSLATE,
    TOOL_ROTATE,
    TOOL_ROTATEZ,
    TOOL_HEIGHT,

    TOOL_COUNT
};


#define TF_CTRLDOWN     (1 << 0)
#define TF_LOCKX        (1 << 1)
#define TF_LOCKY        (1 << 2)
#define TF_LOCKZ        (1 << 3)
#define TF_SNAP         (1 << 4)


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
    int _lastX, _lastY;
    int _lastWheel;
    unsigned int _flags;

    bool getGroundPosition(int screenX, int screenY, Core::Vector3& pos) const;

public:

    ITool(ToolType type);
    virtual ~ITool() {}

    ToolType getType() const { return _type; }

    virtual void begin(
        int mouseX,
        int mouseY,
        int mouseWheel,
        unsigned int flags
    );
    virtual void end() {}
    virtual void cancel() {}

    virtual void onMouseButtonPressed(MouseButton button);
    virtual void onMouseButtonReleased(MouseButton button);
    virtual void onMouseMove(int deltaX, int deltaY);
    virtual void onMouseWheel(int wheel);

    void setFlags(unsigned int flags) { _flags = flags; }
};

}