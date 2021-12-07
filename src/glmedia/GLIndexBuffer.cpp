
#include "GLIndexBuffer.h"
#include "GLDefs.h"
#include <assert.h>
#include "../loguru.hpp"


const size_t g_IndexSize[IDX_COUNT]={
  sizeof(unsigned short),
    sizeof(unsigned int)
};

GLIndexBuffer::GLIndexBuffer(int idxCount, IndexType idxType, BufferUsage bu, const void *data) : IIndexBuffer(idxCount,idxType,bu)
{
#ifdef SGL_TRACE_ALL
   LOG_S(2) << "- [GLIndexBuffer::GLIndexBuffer " << this << "] (" << idxCount << "," << data << ")...";
#endif

    glGenBuffers(1,&_ibId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibId);

    GLenum glbu=GL_STATIC_DRAW;
    switch(bu)
    {
    case BU_DEFAULT:
    case BU_DYNAMIC:
        glbu=GL_STREAM_DRAW;
        break;

    case BU_STAGING:
        glbu=GL_DYNAMIC_DRAW;
        break;
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxCount * g_IndexSize[idxType], data , glbu);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

#ifdef SGL_TRACE_ALL
   LOG_S(2) << "OK (" << _ibId << ")";
#endif
}

GLIndexBuffer::~GLIndexBuffer()
{
    glDeleteBuffers(1,&_ibId);
}

void GLIndexBuffer::set()
{
#ifdef SGL_TRACE_ALL
   LOG_S(2) << "- [GLIndexBuffer::set " << this << "] (" << _ibId << ")...";
#endif

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibId);

#ifdef SGL_TRACE_ALL
   LOG_S(2) << "OK";
#endif
}

void GLIndexBuffer::fill(const void *data, int count, int offset)
{
#ifdef SGL_TRACE_ALL
   LOG_S(2) << "- [GLIndexBuffer::fill " << this << "] (" << _ibId << "," << count << ")...";
#endif

    assert(_usage!=BU_IMMUTABLE);

    unsigned int byte_size=count==0 ? (_idxCount * g_IndexSize[_idxType]) : count * g_IndexSize[_idxType];
    unsigned int byte_offset=offset * g_IndexSize[_idxType];

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibId);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, byte_offset, byte_size, data);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

#ifdef SGL_TRACE_ALL
   LOG_S(2) << "OK";
#endif
}
