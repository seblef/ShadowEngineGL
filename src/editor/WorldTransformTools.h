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

    void onMouseButtonPressed(MouseButton button);
    void onMouseMove(int deltaX, int deltaY);

    void cancel();
};

class RotationTool : public SelectionTool
{
protected:
    Core::Vector3 _rotation;
    std::map<Object*,Core::Vector3> _savedRotations;

    void snap(Object* obj) const;

public:
    RotationTool();
    
    void onMouseButtonPressed(MouseButton button);
    void onMouseMove(int deltaX, int deltaY);

    void cancel();
};

}