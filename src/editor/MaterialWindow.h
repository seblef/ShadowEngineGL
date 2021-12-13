#pragma once

#include "IWindow.h"
#include <chrono>

namespace Editor
{

class EdMaterial;
class EdMaterialSave;
class PreviewMesh;

class MaterialWindow : public IWindow
{
protected:
    EdMaterial* _material;
    EdMaterialSave* _save;
    PreviewMesh* _preview;
    std::chrono::time_point<std::chrono::steady_clock> _startTime;
    bool _changed;

    void drawColors();
    void drawTextures();
    void drawUV();
    void drawBlendAndCull();
    void drawAnimations();
    void drawGame();
    void drawButtons();

public:
    MaterialWindow(EdMaterial* material);
    ~MaterialWindow();

    void draw();
};

}