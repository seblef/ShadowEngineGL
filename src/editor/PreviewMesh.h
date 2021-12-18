#pragma once

#include "PreviewFrame.h"

class Geometry;
class GeometryData;
class IGeometryBuffer;
class Material;

namespace Editor
{


class PreviewMesh : public PreviewFrame
{
protected:

    Material* _material;
    IGeometryBuffer* _geoBuffer;
    GeometryData* _geoData;


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
    PreviewMesh(
        GeometryData* geometry,
        Material* material,
        IVideoDevice* device,
        PreviewResources* resources,
        int width,
        int height
    );
    ~PreviewMesh();

    void setGeometryData(GeometryData* data) { _geoData = data; }
    void setGeometryBuffer(IGeometryBuffer* buffer) { _geoBuffer = buffer; }
    void setMaterial(Material* material) { _material = material; }
};
}