
#include "GLDepthTexture.h"
#include "GLDefs.h"
#include "../loguru.hpp"


GLDepthTexture::GLDepthTexture(int w, int h, int bpp, bool stencil) : IDepthTexture(w,h), _stencil(stencil)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLDepthTexture::GLDepthTexture " << this << "] (" << w << "," << h << "," << bpp << ")...";
#endif

    stencil=false;      //      [TODO] Ajouter la gestion du stencil
    GLint iformat;
/*    if(stencil)
        iformat=GL_DEPTH24_STENCIL8;
    else if(bpp==16)
        iformat=GL_DEPTH_COMPONENT16;
    else*/
        iformat=GL_DEPTH_COMPONENT32;

    glGenTextures(1,&_id);
    glBindTexture(GL_TEXTURE_2D,_id);
    glTexImage2D(GL_TEXTURE_2D,0,iformat,w,h,0,stencil ? GL_DEPTH_STENCIL : GL_DEPTH_COMPONENT,GL_FLOAT,0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK (" << _id << ")";
#endif
}

GLDepthTexture::~GLDepthTexture()
{
    glDeleteTextures(1,&_id);
}

void GLDepthTexture::set(int stage)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLDepthTexture::set " << this << "] (" << _id << ")...";
#endif

    glActiveTexture(GL_TEXTURE0+stage);
    glBindTexture(GL_TEXTURE_2D, _id);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif
}
