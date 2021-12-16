#pragma once

#include "PreviewCamera.h"
#include <memory>

class IDepthTexture;
class IFrameBuffer;
class ITexture;
class IVideoDevice;

namespace Editor
{
class PreviewResources;

class PreviewFrame
{
protected:
    static int _id;
    int _width, _height;
    PreviewCamera _camera;
    IVideoDevice* _device;
    std::unique_ptr<ITexture> _backBuffer;
    std::unique_ptr<IDepthTexture> _depthBuffer;
    std::unique_ptr<IFrameBuffer> _frameBuffer;
    PreviewResources* _resources;

    Core::Vector2 _dragDelta;
    float _wheel;

    virtual void render(float time) = 0;
    void processMouse();

public:
    PreviewFrame(
        IVideoDevice* device,
        PreviewResources* resources,
        int width,
        int height
    );
    ~PreviewFrame();

    void draw(float time);
};

}