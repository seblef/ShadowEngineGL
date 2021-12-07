#include "ShadowMapBuffer.h"
#include "../mediacommon/IConstantBuffer.h"
#include "../mediacommon/IShadowMap.h"
#include "../mediacommon/IVideoDevice.h"

ShadowMapBuffer::ShadowMapBuffer(IVideoDevice* device)
{
    _cBuffer=device->createConstantBuffer(5,3);
}

ShadowMapBuffer::~ShadowMapBuffer()
{
    delete _cBuffer;
}

void ShadowMapBuffer::set()
{
    _cBuffer->set(); 
}

void ShadowMapBuffer::setupLight(IShadowMap *m, int filter, const Matrix4& viewProj)
{
    int f=(int)filter;
    _viewProj=viewProj;
    _invShadowMapSize=Vector2(1.0f / m->getWidth(),1.0f / m->getHeight());
    _shadowFilter=(float)f;
    _invShadowFilter=1.0f / filter;
    _cBuffer->fill(this);
}
