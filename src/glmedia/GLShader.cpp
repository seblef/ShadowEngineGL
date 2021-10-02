
#include "GLShader.h"
#include "GLDefs.h"
#include <iostream>
#include "../loguru.hpp"


GLShader::ShaderDB		GLShader::_db;


GLShader::GLShader(const string &shaderName) : IShader(shaderName)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLShader::GLShader " << this << "] (" << shaderName << ")...";
#endif

    GLuint pid,vid;
    _good=loadShader(shaderName,vid,VSHADER) && loadShader(shaderName,pid,PSHADER);

    if(_good)
    {
        _shaderID=glCreateProgram();
        glAttachShader(_shaderID,vid);
        glAttachShader(_shaderID,pid);
        glLinkProgram(_shaderID);

        GLint res;
        glGetProgramiv(_shaderID,GL_LINK_STATUS,&res);
        if(res==GL_FALSE)
        {
            GLchar error_msg[2048];
            LOG_SCOPE_F(ERROR, "Failed linking shader program %s:", shaderName.c_str());
            glGetProgramInfoLog(_shaderID,2048,0,error_msg);
            LOG_S(ERROR) << error_msg;

            glDeleteProgram(_shaderID);
            _shaderID=0;
        }

        glDeleteShader(vid);
        glDeleteShader(pid);

    }
    _db.insert(make_pair(shaderName, this));

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK (" << _shaderID << ")";
#endif
}

GLShader::~GLShader()
{
    if(_good)
        glDeleteProgram(_shaderID);
}

void GLShader::set()
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLShader::set " << this << "] (" << _shaderID << ")...";
#endif

    glUseProgram(_shaderID);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

}

int GLShader::remRef()
{
    --_refCount;
    if (_refCount <= 0)
    {
        ShaderDB::iterator s(_db.find(_name));
        if (s != _db.end())
            _db.erase(s);

        delete this;
        return 0;
    }
    else
        return _refCount;
}

bool GLShader::loadShader(const string &shader, GLuint& id, int st)
{
    GLuint stgl;
    string stname;
    string stext;

    switch(st)
    {
    case VSHADER:
        stgl=GL_VERTEX_SHADER;
        stname="vertex";
        stext="_vs.glsl";
        break;

    case PSHADER:
        stgl=GL_FRAGMENT_SHADER;
        stname="pixel";
        stext="_ps.glsl";
        break;

    default:
        return false;
    }

    id=glCreateShader(stgl);

    string fileName("Shaders/");
    fileName+=shader;
    fileName+=stext;

    string code;
    if(!getFileCode(fileName.c_str(),code))
    {
        glDeleteShader(id);
        return false;
    }

    const char* source=code.c_str();
    glShaderSource(id,1,&source,0);
    glCompileShader(id);

    GLint res=GL_FALSE;
    GLchar error_msg[2048];

    glGetShaderiv(id,GL_COMPILE_STATUS,&res);
    if(res==GL_FALSE)
    {
        LOG_SCOPE_F(ERROR, "Failed compiling %s shader %s:", stname.c_str(), shader.c_str());
        glGetShaderInfoLog(id,2048,0,error_msg);
        LOG_S(ERROR) << error_msg;
        LOG_S(ERROR) << "Shader code: " << endl << code;
        glDeleteShader(id);
        return false;
    }

    return true;
}

bool GLShader::getFileCode(const string &fileName, string &code)
{
    ifstream vStream(fileName.c_str(),ios::in);
    string line;

    if(vStream.is_open())
    {
        while(getline(vStream,line))
            preprocessLine(line,code);

        vStream.close();
        return true;
    }
    else
    {
        LOG_S(ERROR) << "Failed opening fragment shader file " << fileName;
        return false;
    }
}

bool GLShader::preprocessLine(const string& line, string& code)
{
    const string s_include("#include");
    size_t p=line.find(s_include);
    size_t fn_start,fn_end;

    if(p==string::npos)
    {
        code+=line;
        code+='\n';
        return true;
    }
    else
    {
        p+=s_include.length();
        while(line.c_str()[p]!='"' && p < line.length())
            ++p;

        if(p==line.length())
        {
            LOG_S(ERROR) << "Syntax error in #include command... " << line;
            return false;
        }

        fn_start=p;
        ++p;
        while(line.c_str()[p]!='"' && p < line.length())
            ++p;

        if(p==line.length())
        {
            LOG_S(ERROR) << "Syntax error in #include command... " << line;
            return false;
        }

        fn_end=p;

        string fileName="Shaders/";
        fileName+=line.substr(fn_start+1,fn_end-fn_start-1);

        return getFileCode(fileName,code);
    }
}

void GLShader::clearDB()
{
    ShaderDB::iterator s(_db.begin());
    for (; s != _db.end(); ++s)
        delete s->second;

    _db.clear();
}

GLShader* GLShader::getShaderFromDB(const string& name)
{
    ShaderDB::iterator s(_db.find(name));
    return s == _db.end() ? 0 : s->second;
}
