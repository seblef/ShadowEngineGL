#pragma once

#include "../MediaCommon.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLDepthTexture : public IDepthTexture
{
protected:

    GLuint          _id;
    bool            _stencil;

public:

    GLDepthTexture(int w, int h, int bpp, bool stencil);
    ~GLDepthTexture();

    void            set(int stage);

    GLuint          getId() const                   { return _id; }
    bool            hasStencil() const              { return _stencil; }
};
