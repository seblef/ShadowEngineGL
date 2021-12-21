#pragma once

#include "ITool.h"

namespace Editor
{

class EdCamera;

class CameraTool : public ITool
{
protected:
    EdCamera* _camera;

public:
    CameraTool();

    void begin(
        int mouseX,
        int mouseY,
        int wheel,
        unsigned int flags
    );
    void onMouseMove(int deltaX, int deltaY);
    void onMouseWheel(int deltaWheel);
};

}