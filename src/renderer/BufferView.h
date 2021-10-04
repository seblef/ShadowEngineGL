#pragma once

#include "../MediaCommon.h"

class BufferView
{
protected:

    IVideoDevice* _device;
    DepthStencilState _dsState;
    SamplerState _sState;
    BlendState _blendState;
    RenderState _renderState;
    IShader *_shader;
    IConstantBuffer* _cBuffer;
    IGeometryBuffer *_geoBuffer;
    int _numCols;

    enum BufferType {
        BT_TEXTURE,
        BT_DEPTH,
        BT_SHADOW
    };

    class BufferItem
    {
    public:
        const void* buffer;
        BufferType type;

        BufferItem(const void *buffer_, BufferType type_) : buffer(buffer_), type(type_) {}
        BufferItem(const BufferItem& item) : buffer(item.buffer), type(item.type) {}
    };

    vector<BufferItem> _buffers;

public:

    BufferView(IVideoDevice* device, int numCols);
    ~BufferView();

    void addTexture(const ITexture* texture);
    void addDepth(IDepthTexture* depth);
    void addShadow(IShadowMap* shadow);
    void render();
};