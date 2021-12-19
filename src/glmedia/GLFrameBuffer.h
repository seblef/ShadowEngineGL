#pragma once

#include "../mediacommon/IFrameBuffer.h"
#include <GL/glew.h>

class IDepthTexture;
class ITexture;

class GLFrameBuffer : public IFrameBuffer
{
protected:
    GLuint _fbId;

public:

    GLFrameBuffer(
        int width,
        int height,
        int targetsCount,
        ITexture** targets,
        IDepthTexture* depthBuffer
    );
    ~GLFrameBuffer();

    void set();
};
