#ifndef GLSHADOWMAP_H
#define GLSHADOWMAP_H

#include "../MediaCommon.h"
#include <GL/glew.h>
#include <GL/gl.h>

class GLShadowMap : public IShadowMap
{
protected:

    GLuint          _fbId;
    GLuint          _mapId;

public:

    GLShadowMap(int w, int h);
    ~GLShadowMap();

    void            setAsRenderTarget();
    void            set(int stage);
};

#endif // GLSHADOWMAP_H
