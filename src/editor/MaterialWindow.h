#pragma once

#include "IWindow.h"

namespace Editor
{

class EdMaterial;
class PreviewMesh;

class MaterialWindow : public IWindow
{
protected:
    EdMaterial* _material;
    PreviewMesh* _preview;

public:
    MaterialWindow(EdMaterial* material);
    ~MaterialWindow();

    void draw();
};

}