#pragma once

#include "ITool.h"
#include "SelectionTool.h"
#include "../core/Vector3.h"
#include <map>


namespace Editor
{

class Object;

class TranslationTool : public SelectionTool
{
protected:
    Core::Vector3 _lastPos;
    Core::Vector3 _translation;
    std::map<Object*,Core::Vector3> _savedPositions;

    void snap(Object* obj) const;

public:
    TranslationTool();

    void begin(
        int mouseX,
        int mouseY,
        int mouseWheel,
        unsigned int flags
    );

    void onMouseButtonPressed(MouseButton button);
    void onMouseButtonReleased(MouseButton button);
    void onMouseMove(int deltaX, int deltaY);

    void cancel();
};

class RotationTool : public ITool
{

};

}