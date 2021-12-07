
#include "GLVideoDevice.h"
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"
#include "GLShader.h"
#include "GLTexture.h"
#include "GLFrameBuffer.h"
#include "GLConstantBuffer.h"
#include "GLShadowMap.h"
#include "GLDepthTexture.h"
#include "GLGeometryBuffer.h"
#include "GLDefs.h"
#include "../loguru.hpp"


const GLenum    GLPrimitiveConvers[PT_COUNT]={
    GL_POINTS,
    GL_LINES,
    GL_LINE_STRIP,
    GL_TRIANGLES,
    GL_TRIANGLE_STRIP,
    GL_TRIANGLE_STRIP_ADJACENCY,
    GL_PATCHES,
    GL_PATCHES
};

GLVideoDevice::GLVideoDevice(int width, int height, bool windowed, GLFWwindow* win) : IVideoDevice(width,height,windowed), _win(win),
    _primitiveType(GL_TRIANGLES), _lastVxType(VX_COUNT), _lastBlendState(0), _lastDepthState(0), _lastRenderState(0)
{
    windowed=true;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CW);

    Vx2D v[4]={
        Vx2D(Vector3(-1.0f,-1.0f,0.5f),	Color::White, Vector2(0.0f,0.0f)),
        Vx2D(Vector3( 1.0f,-1.0f,0.5f), Color::White, Vector2(1.0f,0.0f)),
        Vx2D(Vector3( 1.0f, 1.0f,0.5f), Color::White, Vector2(1.0f,1.0f)),
        Vx2D(Vector3(-1.0f, 1.0f,0.5f), Color::White, Vector2(0.0f,1.0f)) };
    unsigned short ind[6]={0, 1, 2, 0, 2, 3};

    _FSGeo=createGeometryBuffer(4,VX_2D,v,6,IDX_16BITS,ind,BU_IMMUTABLE);
    _FSRState=createRenderState(CULL_NONE);

//    ilInit();

    _good=true;
}

GLVideoDevice::~GLVideoDevice()
{
    delete _FSGeo;

    for(auto const& b : _blendStates)
        delete b.second;

    for(auto const& d : _depthStates)
        delete d.second;

    for(auto const& r : _renderStates)
        delete r.second;

    for(auto const& s : _samplerStates)
        glDeleteSamplers(1,&s.second);
}

IVertexBuffer*		GLVideoDevice::createVertexBuffer(int vxCount, VertexType vxType, BufferUsage bu, const void* data) const
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::createVertexBuffer] (" << vxCount << "," << vxType << ")...";
#endif
    return new GLVertexBuffer(vxCount,vxType,bu,data);
}

IIndexBuffer*		GLVideoDevice::createIndexBuffer(int idxCount, IndexType idxType, BufferUsage bu, const void* data) const
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::createIndexBuffer] (" << idxCount << ")...";
#endif
    return new GLIndexBuffer(idxCount,idxType,bu,data);
}

IGeometryBuffer*    GLVideoDevice::createGeometryBuffer(int vxCount, VertexType vxType, const void *vxData,
                                                        int idxCount, IndexType idxType, const void *idxData,
                                                        BufferUsage bu) const
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::createGeometryBuffer] (" << vxCount << "," << vxType << ")...";
#endif

    return new GLGeometryBuffer(vxCount,vxType,vxData,idxCount,idxType,idxData,bu);
}

IShader*			GLVideoDevice::createShader(const string& shaderName) const
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::createShader] (" << shaderName << ")...";
#endif

    GLShader *s = GLShader::getShaderFromDB(shaderName);
    if (s)
    {
        s->addRef();
        return s;
    }
    else
        return new GLShader(shaderName);
}

ITexture* GLVideoDevice::createTexture(
    const string& name,
    int width, int height,
    TextureFormat format,
    BufferUsage bu,
    bool renderTarget,
    bool cubeTexture,
    const void* data
) const
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::createTexture] (" << name << "," << (renderTarget ? "true" : "false") << width << "," << height << ")...";
#endif

    return new GLTexture(name,width,height,format,bu,renderTarget,cubeTexture, data);
}

IDepthTexture*      GLVideoDevice::createDepthTexture(int width, int height, int bpp, bool stencil)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::createDepthTexture] (" << width << "," << height << ")...";
#endif

    return new GLDepthTexture(width,height,bpp,stencil);
}

IFrameBuffer*       GLVideoDevice::createFrameBuffer(int width, int height, int frameCount, ITexture **frames, IDepthTexture *depth)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::createFrameBuffer] (" << width << "," << height << "," << frameCount << "," << (depth==0 ? "no_depth" : "deth") << ")...";
#endif
    return new GLFrameBuffer(width,height,frameCount,frames,depth);
}

IConstantBuffer*	GLVideoDevice::createConstantBuffer(int size, int slot) const
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::createConstantBuffer] (" << size << "," << slot << ")...";
#endif
    return new GLConstantBuffer(size,slot);
}

IShadowMap*			GLVideoDevice::createShadowMap(int width, int height) const
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::createShadowMap] (" << width << "," << height << ")...";
#endif
    return new GLShadowMap(width,height);
}

//			-----------------------
//					States
//			-----------------------

BlendState			GLVideoDevice::createBlendState(bool bBlendEnable, BlendMode SrcBlend, BlendMode DestBlend)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::createBlendState]...";
#endif

    unsigned int k=GLBlendKey(bBlendEnable, SrcBlend, DestBlend);
    GLBlendState* bs=0;
    BlendMap::const_iterator b(_blendStates.find(k));
    if(b==_blendStates.end())
    {
        bs=new GLBlendState(bBlendEnable,g_GLBlendConvers[SrcBlend],g_GLBlendConvers[DestBlend]);
        _blendStates.insert(make_pair(k,bs));
    }
    else
        bs=b->second;

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

    return bs;
}

DepthStencilState	GLVideoDevice::createDepthStencilState(bool bZEnable, bool bZWriteEnable, CompareFunc ZCompare)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::createDepthStencilState]...";
#endif

    u_int64_t k=GLDepthKey(bZEnable,bZWriteEnable,ZCompare);
    GLDepthStencilState* ds=0;
    DepthMap::const_iterator d(_depthStates.find(k));
    if(d==_depthStates.end())
    {
        ds=new GLDepthStencilState(bZEnable,bZWriteEnable,g_GLCompConvers[ZCompare]);
        _depthStates.insert(make_pair(k,ds));
    }
    else
        ds=d->second;

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

    return ds;
}

DepthStencilState	GLVideoDevice::createDepthStencilStateEx(bool bZEnable, bool bZWriteEnable, CompareFunc ZCompare,
    bool bStencilEnable, unsigned char byReadMask, unsigned char byWriteMask,
    CompareFunc cfFrontFunc, StencilOp soFrontStencilFail, StencilOp soFrontDepthFail, StencilOp soFrontStencilPass,
    CompareFunc cfBackFunc, StencilOp soBackStencilFail, StencilOp soBackDepthFail, StencilOp soBackStencilPass)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::createDepthStencilStateEx]...";
#endif

    u_int64_t k=GLDepthKey(bZEnable,bZWriteEnable,ZCompare,bStencilEnable,byReadMask,byWriteMask,
                           cfFrontFunc, soFrontStencilFail, soFrontDepthFail, soFrontStencilPass,
                           cfBackFunc, soBackStencilFail, soBackDepthFail, soBackStencilPass);
    GLDepthStencilState* ds=0;
    DepthMap::const_iterator d(_depthStates.find(k));
    if(d==_depthStates.end())
    {
        ds=new GLDepthStencilState(bZEnable,bZWriteEnable ? GL_TRUE : GL_FALSE,g_GLCompConvers[ZCompare],bStencilEnable,byReadMask,byWriteMask,
                                   g_GLCompConvers[cfFrontFunc], g_GLStencilOpConvers[soFrontStencilFail], g_GLStencilOpConvers[soFrontDepthFail], g_GLStencilOpConvers[soFrontStencilPass],
                                   g_GLCompConvers[cfBackFunc], g_GLStencilOpConvers[soBackStencilFail], g_GLStencilOpConvers[soBackDepthFail], g_GLStencilOpConvers[soBackStencilPass]);
        _depthStates.insert(make_pair(k,ds));
    }
    else
        ds=d->second;

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

    return ds;
}

SamplerState		GLVideoDevice::createSamplerState(FilterMode filter,
    AddressMode uAddress, AddressMode vAddress, AddressMode wAddress)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::createSamplerState]...";
#endif

    unsigned int k=GLSamplerKey(filter,uAddress,vAddress,wAddress);
    GLuint ss=0;
    SamplerMap::const_iterator s(_samplerStates.find(k));
    if(s==_samplerStates.end())
    {
        glGenSamplers(1,&ss);
        glSamplerParameteri(ss, GL_TEXTURE_WRAP_R, g_GLAddressConvers[wAddress]);
        glSamplerParameteri(ss, GL_TEXTURE_WRAP_S, g_GLAddressConvers[uAddress]);
        glSamplerParameteri(ss, GL_TEXTURE_WRAP_T, g_GLAddressConvers[vAddress]);
        glSamplerParameteri(ss, GL_TEXTURE_MAG_FILTER, g_GLFilterConvers[filter]);
        glSamplerParameteri(ss, GL_TEXTURE_MIN_FILTER, g_GLFilterConvers[filter]);

        _samplerStates.insert(make_pair(k,ss));
    }
    else
        ss=s->second;

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

    return (void*)ss;
}

RenderState			GLVideoDevice::createRenderState(CullMode cull, FillMode fill)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::createRenderState]...";
#endif

    unsigned int k=GLRenderKey(cull,fill);
    GLRenderState* rs=0;
    RenderMap::const_iterator r(_renderStates.find(k));
    if(r==_renderStates.end())
    {
        rs=new GLRenderState(cull!=CULL_NONE,g_GLCullConvers[cull],g_GLFillConvers[fill]);
        _renderStates.insert(make_pair(k,rs));
    }
    else
        rs=r->second;

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

    return rs;
}

void				GLVideoDevice::getBlendStateDesc(BlendState bs, bool& enable, BlendMode& src, BlendMode& dest) const
{
    GLBlendState *gbs=(GLBlendState *)bs;
    enable=gbs->_enable;

    for(int i=0;i<BLEND_COUNT;++i)
    {
        if(gbs->_src==g_GLBlendConvers[i])
            src=(BlendMode)i;

        if(gbs->_dest==g_GLBlendConvers[i])
            dest=(BlendMode)i;
    }
}

void				GLVideoDevice::getRenderStateDesc(RenderState rs, CullMode& c, FillMode& f) const
{
    GLRenderState *grs=(GLRenderState *)rs;
    if(grs->_cullEnable)
    {
        for(int i=1;i<CULL_COUNT;++i)
        {
            if(grs->_cull==g_GLCullConvers[i])
                c=(CullMode)i;
        }
    }
    else
        c=CULL_NONE;

    for(int i=0;i<FILL_COUNT;++i)
    {
        if(grs->_fill==g_GLFillConvers[i])
            f=(FillMode)i;
    }
}

void				GLVideoDevice::destroyBlendState(BlendState state) const
{
}

void				GLVideoDevice::destroyDepthStencilState(DepthStencilState state) const
{
}

void				GLVideoDevice::destroySamplerState(SamplerState state) const
{
}

void				GLVideoDevice::destroyRenderState(RenderState state) const
{
}

void				GLVideoDevice::setBlendState(BlendState state)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::SetBlendState]...";
#endif

    if(state!=_lastBlendState)
    {
        GLBlendState *gbs=(GLBlendState*)state;
        if(gbs->_enable)
        {
            glEnable(GL_BLEND);
            glBlendFunc(gbs->_src,gbs->_dest);
        }
        else
            glDisable(GL_BLEND);

        _lastBlendState=state;
    }
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

}

void				GLVideoDevice::setDepthStencilState(DepthStencilState state)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::SetDepthStencilState]...";
#endif

    if(state!=_lastDepthState)
    {
        GLDepthStencilState* gds=(GLDepthStencilState*)state;
        if(gds->_zEnable)
        {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(gds->_depthComp);
        }
        else
            glDisable(GL_DEPTH_TEST);

        glDepthMask(gds->_zWriteEnable);

        if(gds->_stencilEnable)
        {
            glEnable(GL_STENCIL_TEST);
            glStencilFuncSeparate(GL_FRONT,gds->_sFrontFunc,1,gds->_readMask);
            glStencilOpSeparate(GL_FRONT,gds->_sFrontStencilFail,gds->_sFrontDepthFail,gds->_sFrontStencilPass);
            glStencilMaskSeparate(GL_FRONT,gds->_writeMask);

            glStencilFuncSeparate(GL_BACK,gds->_sBackFunc,1,gds->_readMask);
            glStencilOpSeparate(GL_BACK,gds->_sBackStencilFail,gds->_sBackDepthFail,gds->_sBackStencilPass);
            glStencilMaskSeparate(GL_BACK,gds->_writeMask);
        }
        else
            glDisable(GL_STENCIL_TEST);

        _lastDepthState=state;
    }
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

}

void				GLVideoDevice::setSamplerState(int iSlot, SamplerState state) const
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::SetSamplerState]...";
#endif

    glBindSampler(iSlot,(char*)state - (char*)0);
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

}

void				GLVideoDevice::setSamplerState(int iStart, int iCount, SamplerState *pStates) const
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::SetSamplerStateEx]...";
#endif

    for(int i=0;i<iCount;++i)
        glBindSampler(i+iStart,(char*)pStates[i] - (char*)0);
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

}

void				GLVideoDevice::setRenderState(RenderState state)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::SetRenderState]...";
#endif

    if(state!=_lastRenderState)
    {
        GLRenderState *rs=(GLRenderState*)state;
        if(rs->_cullEnable)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(rs->_cull);
        }
        else
            glDisable(GL_CULL_FACE);

        glPolygonMode(GL_FRONT_AND_BACK,rs->_fill);

        _lastRenderState=state;
    }
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

}

void				GLVideoDevice::setPrimitiveType(PrimitiveType pt)
{
    _primitiveType=GLPrimitiveConvers[pt];
}

//			--------------------------
//					Render targets
//			--------------------------

void				GLVideoDevice::resetRenderTargets()
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::resetRenderTargets]...";
#endif

    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glViewport(0,0,_width,_height);
    glDrawBuffer(GL_BACK);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif
}

//			--------------------------
//					Rendering
//			--------------------------

void				GLVideoDevice::newFrame() const
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::newFrame]...";
#endif

    glfwSwapBuffers(_win);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

}

void				GLVideoDevice::clearRenderTargets(const Color& cColor) const
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::clearRenderTargets]...";
#endif

    glClearColor(cColor.r,cColor.g,cColor.b,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

}

void				GLVideoDevice::clearDepthStencil() const
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::clearDepthStencil]...";
#endif

    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

}

void				GLVideoDevice::setInputLayer(VertexType vx)
{
}

void				GLVideoDevice::setStencilRef(unsigned long dwRef)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::setStencilRef]...";
#endif

    GLDepthStencilState* gds=(GLDepthStencilState*)_lastDepthState;
    assert(gds);
    glStencilFuncSeparate(GL_FRONT,gds->_sFrontFunc,dwRef,gds->_readMask);
    glStencilFuncSeparate(GL_BACK,gds->_sBackFunc,dwRef,gds->_readMask);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif
}

void				GLVideoDevice::setViewport(const Vector2& vUL, const Vector2& vDR)
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::setViewport]...";
#endif

    glViewport((int)vUL.x,(int)vUL.y,(int)(vDR.x - vUL.x),(int)(vDR.y - vUL.y));

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

}

void				GLVideoDevice::render(int iCount, int iStart) const
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::render]...";
#endif

    glDrawArrays(_primitiveType,iStart,iCount);

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

}

void				GLVideoDevice::renderIndexed(int iIndexCount, int iIndexStart, int iVertOffset) const
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::renderIndexed]...";
#endif

    glDrawElements(_primitiveType,iIndexCount,GL_UNSIGNED_SHORT,(const void*)(iIndexStart * sizeof(unsigned short)));

#ifdef SGL_TRACE_ALL
    LOG_S(2) << "OK";
#endif

}

void				GLVideoDevice::renderIndexedInstanced(int iCountPerInstance, int iInstCount,
    int iStartIdxLocation, int iBaseVertLocation, int iStartInstLocation) const
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "x [GLVideoDevice::renderIndexedInstanced]...";
#endif
}

void				GLVideoDevice::renderFullscreenQuad()
{
#ifdef SGL_TRACE_ALL
    LOG_S(2) << "- [GLVideoDevice::renderFullscreenQuad]...";
#endif

    setInputLayer(VX_2D);
    setRenderState(_FSRState);
    _FSGeo->set();

    glDisable(GL_DEPTH);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,0);
}
