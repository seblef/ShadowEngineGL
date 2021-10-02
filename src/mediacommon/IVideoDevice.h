#pragma once

#include "VideoDefs.h"
#include "IConstantBuffer.h"
#include "IIndexBuffer.h"
#include "IShader.h"
#include "IShadowMap.h"
#include "ITexture.h"
#include "IVertexBuffer.h"
#include "IFrameBuffer.h"
#include "IDepthTexture.h"
#include "IGeometryBuffer.h"

typedef void* BlendState;
typedef void* DepthStencilState;
typedef void* SamplerState;
typedef void* RenderState;


class IVideoDevice
{
protected:

	bool			_good;
	int				_width, _height;
	bool			_windowed;

public:

	IVideoDevice(int width, int height, bool win) : _good(false), _width(width), _height(height), _windowed(win) {}
	virtual ~IVideoDevice() {}

	bool			isGood() const								{ return _good; }
	void			getResolution(int& w, int& h) const			{ w=_width; h=_height; }
	int				getResWidth() const							{ return _width; }
	int				getResHeight() const						{ return _height; }
	bool			isWindowed() const							{ return _windowed; }

	//			----------------------
	//					Resources
	//			----------------------

	virtual IVertexBuffer*		createVertexBuffer(int vxCount, VertexType vxType, BufferUsage bu, const void* data=0) const=0;
	virtual IIndexBuffer*		createIndexBuffer(int idxCount, IndexType idxType, BufferUsage bu, const void* data=0) const=0;
    virtual IGeometryBuffer*    createGeometryBuffer(int vxCount, VertexType vxType, const void *vxData,
                                                     int idxCount, IndexType idxType, const void* idxData,
                                                     BufferUsage bu) const=0;
	virtual IShader*			createShader(const string& shaderName) const=0;
	virtual ITexture*			createTexture(const string& name, int width=0, int height=0, TextureFormat format=TEXF_A8R8G8B8, BufferUsage=BU_DEFAULT,
		bool renderTarget=false, bool cubeTexture=false) const=0;
    virtual IDepthTexture*      createDepthTexture(int width, int height, int bpp, bool stencil)=0;
    virtual IFrameBuffer*       createFrameBuffer(int width, int height, int frameCount, ITexture** frames, IDepthTexture *depth)=0;
	virtual IConstantBuffer*	createConstantBuffer(int size, int slot) const=0;
	virtual IShadowMap*			createShadowMap(int width, int height) const=0;

	//			-----------------------
	//					States
	//			-----------------------

    virtual BlendState			createBlendState(bool bBlendEnable, BlendMode SrcBlend, BlendMode DestBlend)=0;
    virtual DepthStencilState	createDepthStencilState(bool bZEnable, bool bZWriteEnable, CompareFunc ZCompare)=0;
	virtual DepthStencilState	createDepthStencilStateEx(bool bZEnable, bool bZWriteEnable, CompareFunc ZCompare,
		bool bStencilEnable, unsigned char byReadMask, unsigned char byWriteMask,
		CompareFunc cfFrontFunc, StencilOp soFrontStencilFail, StencilOp soFrontDepthFail, StencilOp soFrontStencilPass,
		CompareFunc cfBackFunc, StencilOp soBackStencilFail, StencilOp soBackDepthFail, StencilOp soBackStencilPass)=0;
	virtual SamplerState		createSamplerState(FilterMode filter,
		AddressMode uAddress=ADDRESS_WRAP, AddressMode vAddress=ADDRESS_WRAP,
        AddressMode wAddress=ADDRESS_WRAP)=0;
    virtual RenderState			createRenderState(CullMode cull, FillMode fill=FILL_SOLID)=0;

	virtual void				getBlendStateDesc(BlendState bs, bool& enable, BlendMode& src, BlendMode& dest) const=0;
	virtual void				getRenderStateDesc(RenderState rs, CullMode& c, FillMode& f) const=0;

	virtual void				destroyBlendState(BlendState state) const=0;
	virtual void				destroyDepthStencilState(DepthStencilState state) const=0;
	virtual void				destroySamplerState(SamplerState state) const=0;
	virtual void				destroyRenderState(RenderState state) const=0;

    virtual void				setBlendState(BlendState state)=0;
	virtual void				setDepthStencilState(DepthStencilState state)=0;
    virtual void				setSamplerState(int iSlot, SamplerState state) const=0;
    virtual void				setSamplerState(int iStart, int iCount, SamplerState *pStates) const=0;
    virtual void				setRenderState(RenderState state)=0;
    virtual void				setPrimitiveType(PrimitiveType pt)=0;

	//			--------------------------
	//					Render targets
	//			--------------------------

	virtual void				resetRenderTargets()=0;

	//			--------------------------
	//					Rendering
	//			--------------------------

	virtual void				newFrame() const=0;
	virtual void				clearRenderTargets(const Color& cColor) const=0;
	virtual void				clearDepthStencil() const=0;

    virtual void				setInputLayer(VertexType vx)=0;
	virtual void				setStencilRef(unsigned long dwRef)=0;
	virtual void				setViewport(const Vector2& vUL, const Vector2& vDR)=0;

	virtual void				render(int iCount=0, int iStart=0) const=0;
	virtual void				renderIndexed(int iIndexCount=0, int iIndexStart=0, int iVertOffset=0) const=0;
	virtual void				renderIndexedInstanced(int iCountPerInstance, int iInstCount,
		int iStartIdxLocation, int iBaseVertLocation, int iStartInstLocation) const =0;
	virtual void				renderFullscreenQuad()=0;
};
