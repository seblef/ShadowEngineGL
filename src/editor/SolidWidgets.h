#pragma once

namespace Editor
{

class EdSolidTemplate;
class PreviewMesh;

class SolidWidgets
{
protected:
    EdSolidTemplate* _solid;
    PreviewMesh* _preview;

public:
    SolidWidgets(
        EdSolidTemplate* solid,
        PreviewMesh* preview
    );

    void setPreview(PreviewMesh* preview) { _preview = preview; }
    bool draw();
};

}