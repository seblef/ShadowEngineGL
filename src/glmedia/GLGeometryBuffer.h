#pragma once

#include "../mediacommon/IGeometryBuffer.h"
#include <GL/glew.h>


class GLGeometryBuffer : public IGeometryBuffer
{
protected:

    GLuint  _vbId;
    GLuint  _vboId;
    GLuint          _ibId;

public:

    GLGeometryBuffer(int vxCount, VertexType vxType, const void* vxData,
                     int idxCount, IndexType idxType, const void* idxData,
                     BufferUsage bu);
    ~GLGeometryBuffer();

    void        set();
    void        fillVertices(const void *data, int count, int offset);
    void        fillIndices(const void* data, int count, int offset);
};
