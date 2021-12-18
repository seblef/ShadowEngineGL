#pragma once

#include "IWindow.h"
#include "SolidWidgets.h"
#include <chrono>


namespace Editor
{

class EdStaticTemplate;
class PreviewMesh;

class StaticWindow : public IWindow
{
protected:
    EdStaticTemplate* _static;
    PreviewMesh* _preview;
    std::chrono::time_point<std::chrono::steady_clock> _startTime;
    bool _changed;

    SolidWidgets _solidWidgets;

public:
    StaticWindow(EdStaticTemplate* stat);
    ~StaticWindow();

    void draw();
};

}