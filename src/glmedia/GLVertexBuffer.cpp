
#include "GLVertexBuffer.h"
#include "GLDefs.h"
#include <assert.h>
#include "../loguru.hpp"


GLVertexBuffer::GLVertexBuffer(int count, VertexType type, BufferUsage bu, const void *data) : IVertexBuffer(count,type,bu)//, _vbId(0)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVertexBuffer::GLVertexBuffer " << this << "] (" << count << "," << type << ")...";
#endif

    glGenVertexArrays(1,&_vbId);
    glBindVertexArray(_vbId);

    glGenBuffers(1,&_vboId);

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

    glBindBuffer(GL_ARRAY_BUFFER,_vboId);

    glBufferData(GL_ARRAY_BUFFER,g_VertexSize[type] * count,data,glbu);

    switch(type)
    {
    case VX_3D:
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vx3D), reinterpret_cast<void*>(offsetof(Vx3D, pos)));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vx3D), reinterpret_cast<void*>(offsetof(Vx3D, norm)));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vx3D), reinterpret_cast<void*>(offsetof(Vx3D, tangent)));
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, sizeof(Vx3D), reinterpret_cast<void*>(offsetof(Vx3D, binorm)));
        glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vx3D), reinterpret_cast<void*>(offsetof(Vx3D, uv)));
        break;

    case VX_3DSKIN:
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vx3DSkin), reinterpret_cast<void*>(offsetof(Vx3DSkin, pos)));
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vx3DSkin), reinterpret_cast<void*>(offsetof(Vx3DSkin, boneWeights)));
        glVertexAttribIPointer(2, 4, GL_INT, sizeof(Vx3DSkin), reinterpret_cast<void*>(offsetof(Vx3DSkin, boneIndices)));
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, sizeof(Vx3DSkin), reinterpret_cast<void*>(offsetof(Vx3DSkin, norm)));
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_TRUE, sizeof(Vx3DSkin), reinterpret_cast<void*>(offsetof(Vx3DSkin, tangent)));
        glVertexAttribPointer(5, 3, GL_FLOAT, GL_TRUE, sizeof(Vx3DSkin), reinterpret_cast<void*>(offsetof(Vx3DSkin, binorm)));
        glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(Vx3DSkin), reinterpret_cast<void*>(offsetof(Vx3DSkin, uv)));
        break;

    case VX_2D:
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vx2D), reinterpret_cast<void*>(offsetof(Vx2D, pos)));
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vx2D), reinterpret_cast<void*>(offsetof(Vx2D, color)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vx2D), reinterpret_cast<void*>(offsetof(Vx2D, uv)));
        break;

    case VX_BASE:
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VxBase), reinterpret_cast<void*>(offsetof(VxBase, pos)));
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VxBase), reinterpret_cast<void*>(offsetof(VxBase, color)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VxBase), reinterpret_cast<void*>(offsetof(VxBase, uv)));
        break;

    }

    for(int i=0;i<g_VertexAttribCount[type];++i)
        glEnableVertexAttribArray(i);

    for(int i=g_VertexAttribCount[type];i<7;++i)
        glDisableVertexAttribArray(i);


#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK (" << _vbId << ")";
#endif

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

GLVertexBuffer::~GLVertexBuffer()
{
     glDeleteBuffers(1,&_vboId);
     glDeleteVertexArrays(1,&_vbId);
}

void GLVertexBuffer::set()
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVertexBuffer::set " << this << "] (" << _vbId << ")...";
#endif

    glBindVertexArray(_vbId);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif
}

void GLVertexBuffer::fill(const void *data, int count, int offset)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVertexBuffer::fill " << this << "] (" << _vbId << "," << data << ")...";
#endif

    assert(_usage!=BU_IMMUTABLE);

    unsigned int byte_size=count==0 ? (_vxCount * g_VertexSize[_vxType]) : count * g_VertexSize[_vxType];
    unsigned int byte_offset=offset * g_VertexSize[_vxType];

    glBindBuffer(GL_ARRAY_BUFFER, _vboId);
    glBufferSubData(GL_ARRAY_BUFFER, byte_offset, byte_size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif
}

