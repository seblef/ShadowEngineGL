
#include "GLGeometryBuffer.h"
#include "GLDefs.h"
#include "../loguru.hpp"


const size_t g_IndexSize[IDX_COUNT]={
  sizeof(unsigned short),
    sizeof(unsigned int)
};

GLGeometryBuffer::GLGeometryBuffer(int vxCount, VertexType vxType, const void *vxData,
                                   int idxCount, IndexType idxType, const void *idxData,
                                   BufferUsage bu) : IGeometryBuffer(vxCount,vxType,idxCount,idxType,bu)
{
#ifdef SGL_TRACE_ALL
   LOG_S(2) << "- [GLGeometryBuffer::GLGeometryBuffer " << this << "]...";
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
    glBufferData(GL_ARRAY_BUFFER,g_VertexSize[vxType] * vxCount,vxData,glbu);

    switch(vxType)
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
        glVertexAttribPointer(2, 4, GL_INT, GL_FALSE, sizeof(Vx3DSkin), reinterpret_cast<void*>(offsetof(Vx3DSkin, boneIndices)));
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

    for(int i=0;i<g_VertexAttribCount[vxType];++i)
        glEnableVertexAttribArray(i);

    for(int i=g_VertexAttribCount[vxType];i<7;++i)
        glDisableVertexAttribArray(i);

#ifdef SGL_TRACE_ALL
   LOG_S(2) << "OK (" << _vbId << ")";
#endif

    glGenBuffers(1,&_ibId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibId);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxCount * g_IndexSize[idxType], idxData , glbu);

    glBindVertexArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER,0);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

GLGeometryBuffer::~GLGeometryBuffer()
{
    glDeleteBuffers(1,&_vboId);
    glDeleteBuffers(1,&_ibId);
    glDeleteVertexArrays(1,&_vbId);
}

void GLGeometryBuffer::set()
{
#ifdef SGL_TRACE_ALL
   LOG_S(2) << "- [GLGeometryBuffer::set " << this << "] (" << _vbId << ")...";
#endif

    glBindVertexArray(_vbId);

#ifdef SGL_TRACE_ALL
   LOG_S(2) << "OK";
#endif
}

void GLGeometryBuffer::fillVertices(const void *data, int count, int offset)
{
#ifdef SGL_TRACE_ALL
   LOG_S(2) << "- [GLGeometryBuffer::fillVertices " << this << "] (" << _vbId << "," << data << ")...";
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

void GLGeometryBuffer::fillIndices(const void *data, int count, int offset)
{
#ifdef SGL_TRACE_ALL
   LOG_S(2) << "- [GLGeometryBuffer::fillIndices " << this << "] (" << _ibId << "," << count << ")...";
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
