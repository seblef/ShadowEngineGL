#pragma once

#include "GLStates.h"
#include <map>

using namespace std;

class GLVideoDevice : public IVideoDevice
{
protected:

    GLFWwindow*         _win;

    GLenum              _primitiveType;

    IGeometryBuffer*    _FSGeo;
    RenderState         _FSRState;

    VertexType          _lastVxType;

    typedef map<unsigned int, GLBlendState*>            BlendMap;
    BlendMap            _blendStates;
    BlendState          _lastBlendState;

    typedef map<u_int64_t, GLDepthStencilState*>        DepthMap;
    DepthMap            _depthStates;
    DepthStencilState   _lastDepthState;

    typedef map<unsigned int, GLRenderState*>           RenderMap;
    RenderMap           _renderStates;
    RenderState         _lastRenderState;

    typedef map<unsigned int, GLuint>                   SamplerMap;
    SamplerMap          _samplerStates;

public:

    GLVideoDevice(int width, int height, bool windowed, GLFWwindow* win);
    ~GLVideoDevice();

    //			----------------------
    //					Resources
    //			----------------------

    IVertexBuffer*		createVertexBuffer(int vxCount, VertexType vxType, BufferUsage bu, const void* data=0) const;
    IIndexBuffer*		createIndexBuffer(int idxCount, IndexType idxType, BufferUsage bu, const void* data=0) const;
    IGeometryBuffer*    createGeometryBuffer(int vxCount, VertexType vxType, const void *vxData,
                                             int idxCount, IndexType idxType, const void *idxData,
                                             BufferUsage bu) const;
    IShader*			createShader(const string& shaderName) const;
    ITexture*			createTexture(
        const string& name,
        int width=0, int height=0,
        TextureFormat format=TEXF_A8R8G8B8,
        BufferUsage bu =BU_DEFAULT,
        bool renderTarget=false,
        bool cubeTexture=false,
        const void* data=0
    ) const;
    IDepthTexture*      createDepthTexture(int width, int height, int bpp, bool stencil);
    IFrameBuffer*       createFrameBuffer(int width, int height, int frameCount, ITexture** frames, IDepthTexture *depth);
    IConstantBuffer*	createConstantBuffer(int size, int slot) const;
    IShadowMap*			createShadowMap(int width, int height) const;

    //			-----------------------
    //					States
    //			-----------------------

    BlendState			createBlendState(bool bBlendEnable, BlendMode SrcBlend, BlendMode DestBlend);
    DepthStencilState	createDepthStencilState(bool bZEnable, bool bZWriteEnable, CompareFunc ZCompare);
    DepthStencilState	createDepthStencilStateEx(bool bZEnable, bool bZWriteEnable, CompareFunc ZCompare,
        bool bStencilEnable, unsigned char byReadMask, unsigned char byWriteMask,
        CompareFunc cfFrontFunc, StencilOp soFrontStencilFail, StencilOp soFrontDepthFail, StencilOp soFrontStencilPass,
        CompareFunc cfBackFunc, StencilOp soBackStencilFail, StencilOp soBackDepthFail, StencilOp soBackStencilPass);
    SamplerState		createSamplerState(FilterMode filter,
        AddressMode uAddress=ADDRESS_WRAP, AddressMode vAddress=ADDRESS_WRAP,
        AddressMode wAddress=ADDRESS_WRAP);
    RenderState			createRenderState(CullMode cull, FillMode fill=FILL_SOLID);

    void				getBlendStateDesc(BlendState bs, bool& enable, BlendMode& src, BlendMode& dest) const;
    void				getRenderStateDesc(RenderState rs, CullMode& c, FillMode& f) const;

    void				destroyBlendState(BlendState state) const;
    void				destroyDepthStencilState(DepthStencilState state) const;
    void				destroySamplerState(SamplerState state) const;
    void				destroyRenderState(RenderState state) const;

    void				setBlendState(BlendState state);
    void				setDepthStencilState(DepthStencilState state);
    void				setSamplerState(int iSlot, SamplerState state) const;
    void				setSamplerState(int iStart, int iCount, SamplerState *pStates) const;
    void				setRenderState(RenderState state);
    void				setPrimitiveType(PrimitiveType pt);

    //			--------------------------
    //					Render targets
    //			--------------------------

    void				resetRenderTargets();

    //			--------------------------
    //					Rendering
    //			--------------------------

    void				newFrame() const;
    void				clearRenderTargets(const Color& cColor) const;
    void				clearDepthStencil() const;

    void				setInputLayer(VertexType vx);
    void				setStencilRef(unsigned long dwRef);
    void				setViewport(const Vector2& vUL, const Vector2& vDR);

    void				render(int iCount=0, int iStart=0) const;
    void				renderIndexed(int iIndexCount=0, int iIndexStart=0, int iVertOffset=0) const;
    void				renderIndexedInstanced(int iCountPerInstance, int iInstCount,
        int iStartIdxLocation, int iBaseVertLocation, int iStartInstLocation) const;
    void				renderFullscreenQuad();
};
