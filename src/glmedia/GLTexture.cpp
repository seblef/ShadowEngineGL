
#include "GLTexture.h"
#include "GLDefs.h"
#include "SOIL.h"
#include "../loguru.hpp"


GLint   g_TexInternalFormat[TEXF_COUNT]={
    GL_RGBA,
    GL_RGBA,
    GL_R16F,
    GL_R32F,
    GL_RG32F,
    GL_RG16F,
    GL_RGBA16F,
    GL_RGBA32F,
    GL_R11F_G11F_B10F
};

GLint   g_TexFormat[TEXF_COUNT]={
    GL_RGBA,
    GL_RGBA,
    GL_RED,
    GL_RED,
    GL_RG,
    GL_RG,
    GL_RGBA,
    GL_RGBA,
    GL_RGB
};

GLenum  g_TexType[TEXF_COUNT]={
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,
    GL_HALF_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_HALF_FLOAT,
    GL_HALF_FLOAT,
    GL_FLOAT,
    GL_HALF_FLOAT
};

const int g_TexelBytes[TEXF_COUNT]={
    4,
    4,
    4,
    8,
    4,
    8,
    16,
    4
};

GLTexture::TextureDB GLTexture::_db;

GLTexture::GLTexture(const string& name, int width, int height, TextureFormat format, BufferUsage bu,
                 bool renderTarget, bool cubeTexture) : ITexture(name,TF_NONE,width,height,format,bu)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLTexture::GLTexture " << this << "] (" << name << "," << width << "," << height << ")...";
#endif

    _db.insert(make_pair(name, this));

    GLenum target=cubeTexture ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;

    glGenTextures(1,&_texId);
    glBindTexture(target,_texId);

    if(width==0)				load(target);
    else						createEmpty(target);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK (" << _texId << ")";
#endif
}

GLTexture::~GLTexture()
{
    glDeleteTextures(1,&_texId);
}

void GLTexture::load(GLenum target)
{
/*    const char *ext=_name.c_str() + _name.length()-3;
    if(strcasecmp(ext,"dds")==0)
        loadDDS(target);
    else if(strcasecmp(ext,"bmp")==0)
        loadBMP(target);*/

/*    ILuint imgId=ilGenImage();
    ilBindImage(imgId);

    if(!ilLoadImage(_name.c_str()))
    {
        LOG_S(2) << "x [GLTexture::load]: DevIL failed loading image " << _name;
        ilBindImage(0);
        ilDeleteImage(imgId);
        return;
    }

    if(!ilConvertImage(IL_RGBA,IL_UNSIGNED_BYTE))
    {
        LOG_S(2) << "x [GLTexture::load]: Failed converting image " << _name << " into RGBA format";
        ilBindImage(0);
        ilDeleteImage(imgId);
    }

    ILubyte* data=ilGetData();
    if(!data)
    {
        LOG_S(2) << "x [GLTexture::load]: Failed getting image data " << _name;
        ilBindImage(0);
        ilDeleteImage(imgId);
        return;
    }

    _width=ilGetInteger(IL_IMAGE_WIDTH);
    _height=ilGetInteger(IL_IMAGE_HEIGHT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    ilBindImage(0);
    ilDeleteImage(imgId);*/
    int channels;
    unsigned char* image=SOIL_load_image(_name.c_str(), &_width, &_height, &channels, SOIL_LOAD_RGBA);
    if(image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA,
                      GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        SOIL_free_image_data(image);
    }
    else
        LOG_S(ERROR) << "SOIL failed loading image " << _name;

}

void GLTexture::loadDDS(GLenum target)
{
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

    unsigned char header[124];

    FILE *fp=fopen(_name.c_str(),"rb");
    if(fp==0)
    {
        LOG_S(ERROR) << "Failed opening file " << _name;
        return;
    }

    char fileCode[4];
    fread(fileCode,1,4,fp);

    if(strncmp(fileCode,"DDS ",4)!=0)
    {
        LOG_S(ERROR) << _name << " is not a dds texture file";
        fclose(fp);
        return;
    }

    fread(&header,124,1,fp);

    unsigned int h=*(unsigned int*)&(header[8]);
    unsigned int w=*(unsigned int*)&(header[12]);
    unsigned int linearSize=*(unsigned int*)&(header[16]);
    unsigned int mipMapCount=*(unsigned int*)&(header[24]);
    unsigned int fourCC=*(unsigned int*)&(header[80]);

    _width=w;
    _height=h;

    unsigned char *buffer;
    unsigned int size;
    size=mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer=new unsigned char[size];

    fread(buffer,1,size,fp);
    fclose(fp);

    unsigned int format;
    switch(fourCC)
    {
    case FOURCC_DXT1:
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    case FOURCC_DXT3:
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;
    case FOURCC_DXT5:
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    default:
        delete[] buffer;
        LOG_S(ERROR) << _name << " Unknown fourCC format";
        return;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    for(unsigned int level=0;level < mipMapCount && (w || h);++level)
    {
        size=((w+3)/4) * ((h+3)/4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D,level,format,w,h,0,size,buffer+offset);

        offset+=size;
        w/=2;
        h/=2;

        if(w < 1)       w=1;
        if(h < 1)       h=1;
    }

    delete[] buffer;

    _flags|=TF_GOOD;
}

void GLTexture::loadBMP(GLenum target)
{
    // Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;
    // Actual RGB data
    unsigned char * data;

    // Open the file
    FILE * file = fopen(_name.c_str(),"rb");
    if(file==0)
    {
        LOG_S(ERROR) << "Failed opening file " << _name;
        return;
    }
    // Read the header, i.e. the 54 first bytes

    // If less than 54 bytes are read, problem
    if (fread(header, 1, 54, file)!=54 )
    {
        LOG_S(ERROR) << _name << " is not a BMP file";
        return;
    }

    // A BMP files always begins with "BM"
    if ( header[0]!='B' || header[1]!='M' )
    {
        LOG_S(ERROR) << _name << " is not a BMP file";
        return;
    }

    // Make sure this is a 24bpp file
    if ( *(int*)&(header[0x1E])!=0  ||
         *(int*)&(header[0x1C])!=24 )
    {
        LOG_S(ERROR) << _name << " is not a 24bpp BMP file";
        return;
    }

    // Read the information about the image
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);

    // Everything is in memory now, the file wan be closed
    fclose (file);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // OpenGL has now copied the data. Free our own version
    delete [] data;
    glGenerateMipmap(GL_TEXTURE_2D);
}

void GLTexture::createEmpty(GLenum target)
{
    glTexImage2D(target,0,g_TexInternalFormat[_format],_width,_height,0,g_TexFormat[_format],g_TexType[_format],0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    _flags=TF_GOOD | TF_RENDERTARGET;
}

void GLTexture::set(int stage)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLTexture::set " << this << "] (" << _texId << "," << stage<< ")...";
#endif

    glActiveTexture(GL_TEXTURE0+stage);
    glBindTexture(GL_TEXTURE_2D, _texId);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif
}

bool GLTexture::lock(void **ppData, int& rowPitch, int& depthPitch)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "x [GLTexture::lock " << this << "]";
#endif
    //  [TODO]  Implémenter ici
    return false;
}

void GLTexture::unlock()
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "x [GLTexture::unlock " << this << "]";
#endif
    //  [TODO]  Implémenter ici
}

void GLTexture::fill(const void* data)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "x [GLTexture::fill " << this << "]";
#endif
    //  [TODO]  Implémenter ici
}

bool GLTexture::save(const string& fileName)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "x [GLTexture::save " << this << "]";
#endif
    //  [TODO]  Implémenter ici
    return false;
}

bool GLTexture::generateMipMaps()
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "x [GLTexture::generateMipMaps " << this << "]";
#endif
    //  [TODO]  Implémenter ici
    return false;
}

int GLTexture::remRef()
{
    --_refCount;

    if (_refCount <= 0)
    {
        TextureDB::iterator t(_db.find(_name));
        if (t != _db.end())
            _db.erase(t);

        delete this;
        return 0;
    }
    else
        return _refCount;
}

GLTexture* GLTexture::getTextureFromDB(const string& name)
{
    TextureDB::iterator t(_db.find(name));
    return t == _db.end() ? 0 : t->second;
}

void GLTexture::clearDB()
{
    TextureDB::iterator t(_db.begin());
    for (; t != _db.end(); ++t)
        delete t->second;

    _db.clear();
}
