#pragma once

#include "../MediaCommon.h"
#include <GL/glew.h>
#include <GL/gl.h>

class GLVertexBuffer : public IVertexBuffer
{
protected:

    GLuint  _vbId;
    GLuint  _vboId;

public:

    GLVertexBuffer(int count, VertexType type, BufferUsage bu, const void *data);
    ~GLVertexBuffer();

    void	set();
    void	fill(const void* data, int count=0, int offset=0);

};
