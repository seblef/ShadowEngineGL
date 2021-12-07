
#include "GLShadowMap.h"
#include "GLDefs.h"
#include "../loguru.hpp"


GLShadowMap::GLShadowMap(int w, int h) : IShadowMap(w,h)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLShadowMap::GLShadowMap " << this << "] (" << w << "," << h << ")...";
#endif

    glGenFramebuffers(1,&_fbId);
    glBindFramebuffer(GL_FRAMEBUFFER,_fbId);

    glGenTextures(1,&_mapId);
    glBindTexture(GL_TEXTURE_2D,_mapId);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT16,w,h,0,GL_DEPTH_COMPONENT,GL_FLOAT,0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _mapId, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_S(ERROR) << "Failed creating frame buffer";

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK (" << _fbId << ")";
#endif
}

GLShadowMap::~GLShadowMap()
{
    glDeleteTextures(1,&_mapId);
    glDeleteFramebuffers(1,&_fbId);
}

void GLShadowMap::setAsRenderTarget()
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLShadowMap::setAsRenderTarget " << this << "] (" << _fbId << ")...";
#endif

    glBindFramebuffer(GL_FRAMEBUFFER,_fbId);
    glViewport(0,0,_width,_height);

    glDrawBuffer(GL_NONE);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

}

void GLShadowMap::set(int stage)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLShadowMap::set " << this << "] (" << _fbId << "," << stage << ")...";
#endif

    glActiveTexture(GL_TEXTURE0+stage);
    glBindTexture(GL_TEXTURE_2D, _mapId);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

}
