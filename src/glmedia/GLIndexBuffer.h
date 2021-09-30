#ifndef GLINDEXBUFFER_H
#define GLINDEXBUFFER_H

#include "../MediaCommon.h"
#include <GL/glew.h>
#include <GL/gl.h>

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

#endif // GLINDEXBUFFER_H
