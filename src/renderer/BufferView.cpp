#include "BufferView.h"
#include "../mediacommon/IShader.h"
#include "../mediacommon/IVideoDevice.h"
#include "../mediacommon/IConstantBuffer.h"
#include "../mediacommon/IGeometryBuffer.h"
#include "../mediacommon/IDepthTexture.h"
#include "../mediacommon/ITexture.h"
#include "../mediacommon/IShadowMap.h"


BufferView::BufferView(IVideoDevice* device, int numCols) :
    _device(device),
    _shader(0),
    _cBuffer(0),
    _geoBuffer(0),
    _numCols(numCols)
{
    _dsState = device->createDepthStencilState(false, false, COMP_LESSEQUAL);
    _sState = device->createSamplerState(FILTER_POINT);
    _renderState = device->createRenderState(CULL_NONE);
    _blendState = device->createBlendState(false, BLEND_ONE, BLEND_ZERO);

    _shader = device->createShader("Base/RenderView");
    _cBuffer = device->createConstantBuffer(1, 0);

    const float size = 1.0f / (float)numCols;

    const Vx2D verts[4] = {
        Vx2D(Vector3(-size, -size, 0.5f), Color::White, Vector2(0.f, 0.f)),
        Vx2D(Vector3( size, -size, 0.5f), Color::White, Vector2(1.f, 0.f)),
        Vx2D(Vector3( size,  size, 0.5f), Color::White, Vector2(1.f, 1.f)),
        Vx2D(Vector3(-size,  size, 0.5f), Color::White, Vector2(0.f, 1.f)),
    };
    const unsigned short ind[6] = {0, 1, 2, 0, 2, 3};

    _geoBuffer = device->createGeometryBuffer(4, VX_2D, verts, 6, IDX_16BITS, ind, BU_IMMUTABLE);
}

BufferView::~BufferView()
{
    _shader->remRef();
    delete _geoBuffer;
    delete _cBuffer;

    _device->destroyBlendState(_blendState);
    _device->destroyRenderState(_renderState);
    _device->destroySamplerState(_sState);
    _device->destroyDepthStencilState(_dsState);
}

void BufferView::addTexture(const ITexture* texture)
{
    _buffers.push_back(BufferItem((const void*)texture, BT_TEXTURE));
}

void BufferView::addDepth(IDepthTexture* depth)
{
    _buffers.push_back(BufferItem((const void*)depth, BT_DEPTH));
}

void BufferView::addShadow(IShadowMap* shadow)
{
    _buffers.push_back(BufferItem((const void*)shadow, BT_SHADOW));
}

void BufferView::render()
{
    if(_buffers.empty())
        return;

    _device->setBlendState(_blendState);
    _device->setRenderState(_renderState);
    _device->setSamplerState(0, _sState);
    _device->setDepthStencilState(_dsState);

    _shader->set();
    _cBuffer->set();
    _geoBuffer->set();

    _device->resetRenderTargets();

    const float size = 2.0f / (float)_numCols;

    Vector4 offset(size * .5f - 1.f, size * .5f - 1.f, .0f, .0f);
    unsigned int idxBuffer = 0;
    for(const auto& buffer: _buffers)
    {
        _cBuffer->fill((const void*)&offset);
        switch(buffer.type)
        {
        case BT_TEXTURE:
            ((ITexture*)buffer.buffer)->set(0);
            break;
        case BT_DEPTH:
            ((IDepthTexture*)buffer.buffer)->set(0);
            break;
        case BT_SHADOW:
            ((IShadowMap*)buffer.buffer)->set(0);
            break;
        }

        _device->renderIndexed(6);

        ++idxBuffer;

        if(idxBuffer >= _numCols)
        {
            idxBuffer = 0;
            offset.x = size * .5f - 1.0f;
            offset.y += size;
        }
        else
            offset.x += size;
    }

    _buffers.clear();
}