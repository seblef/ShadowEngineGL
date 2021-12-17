#include "GLFrameBuffer.h"
#include "GLDepthTexture.h"
#include "GLTexture.h"
#include "GLDefs.h"
#include "../loguru.hpp"
#include <assert.h>


GLFrameBuffer::GLFrameBuffer(int w, int h, int fcount, ITexture **f, IDepthTexture *d) : IFrameBuffer(w,h,d), _frameCount(fcount)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLFrameBuffer::GLFrameBuffer " << this << "] (" << w << "," << h << "," << fcount << ")...";
#endif

    assert(fcount <= MAX_RENDER_TARGETS);

    glGenFramebuffers(1,&_fbId);
    glBindFramebuffer(GL_FRAMEBUFFER,_fbId);

    if(d)
    {
        GLDepthTexture *dt=(GLDepthTexture*)d;
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dt->getId(), 0);

//        if(dt->hasStencil())
//            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, ((GLDepthTexture*)d)->getId(), 0);
    }

    GLenum buffers[MAX_RENDER_TARGETS];

    for(int i=0;i<fcount;++i)
    {
        assert(w==f[i]->getWidth() && h==f[i]->getHeight() && f[i]->isRenderTarget());
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, ((GLTexture*)f[i])->getGLId(), 0);
        buffers[i]=GL_COLOR_ATTACHMENT0+i;
    }

    if(fcount > 0)
    {
        glDrawBuffers(fcount,buffers);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG_S(2) << "x [GLFrameBuffer::GLFrameBuffer]: Failed creating frame buffer (" << w << "," << h << "...";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLFrameBuffer::GLFrameBuffer OK (" << _fbId << ")";
#endif
}

GLFrameBuffer::~GLFrameBuffer()
{
    glDeleteFramebuffers(1,&_fbId);
}

void GLFrameBuffer::set()
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLFrameBuffer::set " << this << "] (" << _fbId << ")...";
#endif

    glBindFramebuffer(GL_FRAMEBUFFER,_fbId);
    glViewport(0,0,_width,_height);

    if(_frameCount==0)
        glDrawBuffer(GL_NONE);
    else
    {
        GLenum buffers[MAX_RENDER_TARGETS];

        for(int i=0;i<_frameCount;++i)
            buffers[i]=GL_COLOR_ATTACHMENT0+i;

        glDrawBuffers(_frameCount,buffers);
    }

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif
}
