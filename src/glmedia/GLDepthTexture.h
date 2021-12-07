#pragma once

#include "../mediacommon/IDepthTexture.h"
#include <GL/glew.h>

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
