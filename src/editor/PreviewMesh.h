#pragma once

#include "PreviewCamera.h"

class Geometry;
class IDepthTexture;
class IFrameBuffer;
class IGeometryBuffer;
class ITexture;
class IVideoDevice;
class Material;

namespace Editor
{

class PreviewResources;


class PreviewMesh
{
protected:

    static int _id;

    int _width;
    int _height;

    PreviewCamera _camera;
    Material* _material;

    IVideoDevice* _device;
    ITexture* _backBuffer;
    IDepthTexture* _depthBuffer;
    IFrameBuffer* _frameBuffer;
    IGeometryBuffer* _geoBuffer;

    PreviewResources* _resources;

    Core::Vector2 _dragDelta;
    float _wheel;

    void render(float time);
    void processMouse();

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

    void draw(float time);
};
}