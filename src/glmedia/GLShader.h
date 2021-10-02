#pragma once

#include "../MediaCommon.h"
#include <GL/glew.h>
#include <GL/gl.h>

class GLShader : public IShader
{
protected:

    GLuint          _shaderID;

    typedef map<string, GLShader*>	ShaderDB;
    static ShaderDB				_db;

#define VSHADER                 0
#define PSHADER                 1

    bool						loadShader(const string& shader, GLuint& id, int st);
    bool                        preprocessLine(const string& line, string& code);
    bool                        getFileCode(const string& fileName, string& code);

public:

    GLShader(const string& shaderName);
    ~GLShader();

    void            set();

    int             addRef()                { return ++_refCount; }
    int             remRef();

    GLuint          getShaderId() const     { return _shaderID; }

    static GLShader*			getShaderFromDB(const string& name);
    static void					clearDB();
};
