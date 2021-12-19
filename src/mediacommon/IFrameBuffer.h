#pragma once

class IDepthTexture;
class ITexture;

class IFrameBuffer
{
protected:

    int _width;
    int _height;

    int _targetsCount;
    ITexture** _targets;
    IDepthTexture* _depthBuffer;

public:

    IFrameBuffer(
        int width,
        int height,
        int targetsCount,
        ITexture** targets,
        IDepthTexture* depthBuffer
    ) :
        _width(width),
        _height(height),
        _targetsCount(targetsCount),
        _targets(targets),
        _depthBuffer(depthBuffer)
    {}
    virtual ~IFrameBuffer() {}

    virtual void set()=0;

    int getWidth() const { return _width; }
    int getHeight() const { return _height; }

    int getTargetCount() const { return _targetsCount; }
    ITexture** getTargets() const { return _targets; }
    IDepthTexture* getDepthBuffer() const { return _depthBuffer; }
};
