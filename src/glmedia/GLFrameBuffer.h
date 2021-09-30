#ifndef GLFRAMEBUFFER_H
#define GLFRAMEBUFFER_H

#include "../MediaCommon.h"
#include <GL/glew.h>
#include <GL/gl.h>

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

#endif // GLFRAMEBUFFER_H
