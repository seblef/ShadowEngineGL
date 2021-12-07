#pragma once

#include "../mediacommon/IFrameBuffer.h"
#include <GL/glew.h>

class IDepthTexture;
class ITexture;

class GLFrameBuffer : public IFrameBuffer
{
protected:

    GLuint          _fbId;
    int             _frameCount;

public:

    GLFrameBuffer(int w, int h, int fcount, ITexture** f, IDepthTexture* d);
    ~GLFrameBuffer();

    void            set();
};
