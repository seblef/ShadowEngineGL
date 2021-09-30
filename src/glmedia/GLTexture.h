#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "../MediaCommon.h"
#include <GL/glew.h>
#include <GL/gl.h>

class GLTexture : public ITexture
{
protected:

    GLuint          _texId;

    typedef map<string, GLTexture*>	TextureDB;
    static TextureDB			_db;

    void						loadDDS(GLenum target);
    void                        loadBMP(GLenum target);
    void                        load(GLenum target);
    void						createRenderTarget(GLenum target);
    void						createEmpty(GLenum target);


public:

    GLTexture(const string& name, int width, int height, TextureFormat format, BufferUsage bu, bool renderTarget, bool cubeTexture);
    ~GLTexture();

    void			set(int stage);
    bool			lock(void **ppData, int& rowPitch, int& depthPitch);
    void			unlock();
    void			fill(const void* data);
    bool			save(const string& fileName);
    bool			generateMipMaps();

    int				addRef()                { return ++_refCount; }
    int				remRef();

    GLuint          getGLId() const         { return _texId; }

    static GLTexture*   getTextureFromDB(const string& texName);
    static void         clearDB();
};

#endif // GLTEXTURE_H
