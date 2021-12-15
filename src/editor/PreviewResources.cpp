#include "PreviewResources.h"
#include "../game/GeometryLoader.h"
#include "../mediacommon/IConstantBuffer.h"
#include "../mediacommon/IGeometryBuffer.h"
#include "../mediacommon/IShader.h"
#include "../mediacommon/ITexture.h"
#include <memory>

namespace Editor
{

PreviewResources::PreviewResources(IVideoDevice* device) :
    _device(device),
    _backgroundShader(0),
    _meshShader(0),
    _cBuffer(0),
    _backgroundTexture(0),
    _sphereGeometry(0),
    _backgroundBlendState(0),
    _backgroundDepthState(0),
    _meshDepthState(0)
{
    _backgroundShader = device->createShader("Editor/Background");
    _meshShader = device->createShader("Editor/Material");
    _particlesShader = device->createShader("Base/Particles");

    _backgroundTexture = device->createTexture("Textures/Editor/MaterialBack.dds");
    _cBuffer = device->createConstantBuffer(7, 0);
    _backgroundBlendState = device->createBlendState(false, BLEND_ONE, BLEND_ZERO);
    _backgroundDepthState = device->createDepthStencilState(false, false, COMP_ALWAYS);
    _meshDepthState = device->createDepthStencilState(true, true, COMP_LESSEQUAL);

    std::unique_ptr<Geometry> geo(GeometryLoader::loadGeometry("Geometry/Editor/Sphere.GEO"));

    _sphereGeometry = device->createGeometryBuffer(
        geo->getVertexCount(),
        VX_3D,
        geo->getVertices(),
        geo->getTriangleCount() * 3,
        IDX_16BITS,
        geo->getTriangles(),
        BU_IMMUTABLE
    );
    _sphereBBox = geo->getBBox();
}

PreviewResources::~PreviewResources()
{
    if(_backgroundShader)
        delete _backgroundShader;
    if(_meshShader)
        delete _meshShader;
    if(_cBuffer)
        delete _cBuffer;
    if(_backgroundTexture)
        delete _backgroundTexture;
    if(_sphereGeometry)
        delete _sphereGeometry;
    if(_cBuffer)
        delete _cBuffer;

	_device->destroyBlendState(_backgroundBlendState);
	_device->destroyDepthStencilState(_backgroundDepthState);
	_device->destroyDepthStencilState(_meshDepthState);
}

}