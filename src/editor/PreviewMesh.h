#pragma once

#include "PreviewFrame.h"

class Geometry;
class IGeometryBuffer;
class Material;

namespace Editor
{


class PreviewMesh : public PreviewFrame
{
protected:

    Material* _material;
    IGeometryBuffer* _geoBuffer;

    void render(float time);

public:

    PreviewMesh(
        Geometry* geometry,
        Material* material,
        IVideoDevice* device,
        PreviewResources* resources,
        int width,
        int height
    );
    ~PreviewMesh();
};
}