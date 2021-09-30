#include "GLConstantBuffer.h"
#include "../loguru.hpp"



GLConstantBuffer::GLConstantBuffer(int size, int slot) : IConstantBuffer(size,slot)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLConstantBuffer::GLConstantBuffer " << this << "] (" << size << "," << slot << ")...";
#endif

    glGenBuffers(1,&_cbId);
    glBindBuffer(GL_UNIFORM_BUFFER,_cbId);
    glBufferData(GL_UNIFORM_BUFFER,4*size*sizeof(float),0,GL_STREAM_DRAW);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK (" << _cbId << ")" ;
#endif
}

GLConstantBuffer::~GLConstantBuffer()
{
    glDeleteBuffers(1,&_cbId);
}

void GLConstantBuffer::set()
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLConstantBuffer::set " << this << "] (" << _cbId << ")...";
#endif
    glBindBufferRange(GL_UNIFORM_BUFFER,_slot,_cbId,0,_size*4*sizeof(float));

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK" ;
#endif
}

void GLConstantBuffer::fill(const void *data)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLConstantBuffer::fill " << this << "] (" << _cbId << "," << data << ")...";
#endif
    unsigned int s=_size*4*sizeof(float);
    glBindBuffer(GL_UNIFORM_BUFFER,_cbId);
    glBufferSubData(GL_UNIFORM_BUFFER,0,s,data);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK" ;
#endif
}
