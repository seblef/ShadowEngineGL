#pragma once

#include "../mediacommon/IIndexBuffer.h"
#include <GL/glew.h>


class GLIndexBuffer : public IIndexBuffer
{
protected:

    GLuint          _ibId;

public:

    GLIndexBuffer(int idxCount, IndexType idxType,
                  BufferUsage bu, const void *data=0);
    ~GLIndexBuffer();

    void            set();
    void            fill(const void* data, int count, int offset);

};
