#pragma once

#include "../mediacommon/IConstantBuffer.h"
#include "GLDefs.h"
#include <GL/glew.h>


class GLConstantBuffer : public IConstantBuffer
{
protected:

    GLuint          _cbId;

public:

    GLConstantBuffer(int size, int slot);
    ~GLConstantBuffer();

    void            set();
    void            fill(const void *data);
};
