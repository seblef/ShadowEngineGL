#pragma once

#include "../core/BBox3.h"
#include "../mediacommon/IVideoDevice.h"


namespace Editor
{

class PreviewResources
{
protected:
    IVideoDevice* _device;

    IShader* _backgroundShader;
    IShader* _meshShader;

    IConstantBuffer* _cBuffer;

    ITexture* _backgroundTexture;

    IGeometryBuffer *_sphereGeometry;

    BlendState _backgroundBlendState;
    DepthStencilState _backgroundDepthState;
    DepthStencilState _meshDepthState;

    BBox3 _sphereBBox;

public:
    PreviewResources(IVideoDevice* device);
    ~PreviewResources();

    IVideoDevice* getDevice() const { return _device; }

    IShader* getBackgroundShader() const { return _backgroundShader; }
    IShader* getMeshShader() const { return _meshShader; }

    IConstantBuffer* getConstantBuffer() const { return _cBuffer; }

    ITexture* getBackgroundTexture() const { return _backgroundTexture; }

    IGeometryBuffer* getSphereGeometry() const { return _sphereGeometry; }
    const BBox3& getSphereBBox() const { return _sphereBBox; }

    BlendState getBakcgroundBlendState() const { return _backgroundBlendState; }
    DepthStencilState getBackgroundDepthState() const { return _backgroundDepthState; }
    DepthStencilState getMeshDepthState() const { return _meshDepthState; }
};

}