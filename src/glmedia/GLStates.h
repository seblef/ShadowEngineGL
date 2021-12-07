#pragma once

#include "../mediacommon/IVideoDevice.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdint.h>

#define OFF32(x,o)          (((unsigned int)x) << o)
#define OFF64(x,o)          (((u_int64_t)x) << o)


/*
 *
 *                  Blending
 *
 *
*/

class GLBlendState
{
public:

    bool            _enable;
    GLenum          _src;
    GLenum          _dest;

    GLBlendState(bool enable, GLenum s, GLenum d) : _enable(enable), _src(s), _dest(d)          {}
    GLBlendState(const GLBlendState& s) : _enable(s._enable), _src(s._src), _dest(s._dest)      {}
};

const GLenum  g_GLBlendConvers[BLEND_COUNT]=
{
    GL_ZERO,
    GL_ONE,
    GL_SRC_COLOR,
    GL_ONE_MINUS_SRC_COLOR,
    GL_SRC_ALPHA,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_DST_ALPHA,
    GL_ONE_MINUS_DST_ALPHA,
    GL_DST_COLOR,
    GL_ONE_MINUS_DST_COLOR,
    GL_CONSTANT_ALPHA
};

inline unsigned int GLBlendKey(bool enable, BlendMode s, BlendMode d)
{
    unsigned int k=enable ? 1 : 0;
    return k | OFF32(s,1) | OFF32(d,10);
}

/*
 *
 *                  Depth-stencil
 *
 *
*/

class GLDepthStencilState
{
public:

    bool            _zEnable;
    GLboolean       _zWriteEnable;
    GLenum          _depthComp;

    bool            _stencilEnable;
    unsigned char   _readMask;
    unsigned char   _writeMask;

    GLenum          _sFrontFunc;
    GLenum          _sFrontStencilFail;
    GLenum          _sFrontDepthFail;
    GLenum          _sFrontStencilPass;

    GLenum          _sBackFunc;
    GLenum          _sBackStencilFail;
    GLenum          _sBackDepthFail;
    GLenum          _sBackStencilPass;

    GLDepthStencilState(bool zenable,GLboolean zwenable,GLenum dcomp) : _zEnable(zenable), _zWriteEnable(zwenable),
        _depthComp(dcomp), _stencilEnable(false), _readMask(0),_writeMask(0),
        _sFrontFunc(0), _sFrontStencilFail(0), _sFrontDepthFail(0), _sFrontStencilPass(0),
        _sBackFunc(0), _sBackStencilFail(0), _sBackDepthFail(0), _sBackStencilPass(0)               {}
    GLDepthStencilState(bool zenable,GLboolean zwenable,GLenum dcomp, bool senable, unsigned char rmask, unsigned char wmask,
                        GLenum ff, GLenum fsf, GLenum fdf, GLenum fsp,
                        GLenum bf, GLenum bsf, GLenum bdf, GLenum bsp) : _zEnable(zenable), _zWriteEnable(zwenable),
        _depthComp(dcomp), _stencilEnable(senable), _readMask(rmask),_writeMask(wmask),
        _sFrontFunc(ff), _sFrontStencilFail(fsf), _sFrontDepthFail(fdf), _sFrontStencilPass(fsp),
        _sBackFunc(bf), _sBackStencilFail(bsf), _sBackDepthFail(bdf), _sBackStencilPass(bsp)               {}
    GLDepthStencilState(const GLDepthStencilState& s) : _zEnable(s._zEnable), _zWriteEnable(s._zWriteEnable),
        _depthComp(s._depthComp), _stencilEnable(s._stencilEnable), _readMask(s._readMask), _writeMask(s._writeMask),
        _sFrontFunc(s._sFrontFunc), _sFrontStencilFail(s._sFrontStencilFail), _sFrontDepthFail(s._sFrontDepthFail), _sFrontStencilPass(s._sFrontStencilPass),
        _sBackFunc(s._sBackFunc), _sBackStencilFail(s._sBackStencilFail), _sBackDepthFail(s._sBackDepthFail), _sBackStencilPass(s._sBackStencilPass)        {}
};

const GLenum    g_GLCompConvers[COMP_COUNT]={
    GL_NEVER,
    GL_LESS,
    GL_EQUAL,
    GL_LEQUAL,
    GL_GREATER,
    GL_GEQUAL,
    GL_ALWAYS
};

const GLenum    g_GLStencilOpConvers[STENCIL_COUNT]={
    GL_KEEP,
    GL_REPLACE,
    GL_INCR,
    GL_DECR,
    GL_INVERT,
    GL_INCR_WRAP,
    GL_DECR_WRAP
};

#define OFF64(x,o)          (((u_int64_t)x) << o)

inline u_int64_t GLDepthKey(bool zenable, bool zwenable, CompareFunc dcomp)
{
    u_int64_t k=zenable ? 1 : 0;
    k|=zwenable ? 2 : 0;
    return k | OFF64(dcomp,2);
}

inline u_int64_t GLDepthKey(bool zenable, bool zwenable, CompareFunc dcomp,
                            bool senable, unsigned char rm, unsigned char wm,
                            CompareFunc ff, StencilOp fsf, StencilOp fdf, StencilOp fsp,
                            CompareFunc bf, StencilOp bsf, StencilOp bdf, StencilOp bsp)
{
    u_int64_t k=zenable ? 1 : 0;
    k|=zwenable ? 2 : 0;
    k|=OFF64(dcomp,2);
    k|=senable ? (1 << 5) : 0;
    k|=OFF64(rm,6) | OFF64(wm,14);
    k|=OFF64(ff,22) | OFF64(fsf,25) | OFF64(fdf,28) | OFF64(fsp,31);
    k|=OFF64(bf,34) | OFF64(bsf,37) | OFF64(bdf,40) | OFF64(bsp,43);

    return k;
}

/*
 *
 *                  Render state
 *
 *
*/

class GLRenderState
{
public:

    bool            _cullEnable;
    GLenum          _cull;
    GLenum          _fill;

    GLRenderState(bool cullEnable, GLenum cull, GLenum fill) : _cullEnable(cullEnable), _cull(cull), _fill(fill)      {}
    GLRenderState(const GLRenderState& rs) : _cullEnable(rs._cullEnable), _cull(rs._cull), _fill(rs._fill)   {}
};

const GLenum    g_GLCullConvers[CULL_COUNT]={
    0,
    GL_FRONT,
    GL_BACK
};

const GLenum    g_GLFillConvers[FILL_COUNT]={
    GL_FILL,
    GL_LINE,
    GL_POINT
};


inline unsigned int GLRenderKey(CullMode cm, FillMode fm)
{
    return OFF32(cm,0) | OFF32(fm,8);
}

/*
 *
 *                  Sampler state
 *
 *
*/

const GLint     g_GLFilterConvers[FILTER_COUNT]={
    GL_NEAREST,
    GL_LINEAR,
    GL_LINEAR_MIPMAP_LINEAR
};

const GLint     g_GLAddressConvers[ADDRESS_COUNT]={
        GL_REPEAT,
        GL_MIRRORED_REPEAT,
        GL_CLAMP_TO_EDGE,
        GL_CLAMP_TO_EDGE,
        GL_MIRROR_CLAMP_TO_EDGE
};

inline unsigned int GLSamplerKey(FilterMode fm, AddressMode ua, AddressMode va, AddressMode wa)
{
    return OFF32(fm,0) | OFF32(ua,2) | OFF32(va,6) | OFF32(wa,10);
}
