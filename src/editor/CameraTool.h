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
    CameraTool(
        EdCamera* camera,
        int mouseX,
        int mouseY,
        int mouseWheel
    );

    void onMouseMove(int deltaX, int deltaY);
    void onMouseWheel(int deltaWheel);
};

}