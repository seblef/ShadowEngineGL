#include "PreviewMesh.h"
#include "PreviewResources.h"
#include "../mediacommon/IConstantBuffer.h"
#include "../mediacommon/IGeometryBuffer.h"
#include "../mediacommon/IShader.h"
#include "../mediacommon/IVideoDevice.h"
#include "../game/Geometry.h"
#include "../renderer/Material.h"
#include "../renderer/MaterialSystem.h"

namespace Editor
{


PreviewMesh::PreviewMesh(
    Geometry* geometry,
    Material* material,
    IVideoDevice* device,
    PreviewResources* resources,
    int width,
    int height
) : 
    PreviewFrame(
        device,
        resources,
        width, height
    ),
    _material(material),
    _geoBuffer(0)
{
    if(geometry)
    {
        _geoBuffer = device->createGeometryBuffer(
            geometry->getVertexCount(),
            VX_3D,
            geometry->getVertices(),
            geometry->getTriangleCount() * 3,
            IDX_16BITS,
            geometry->getTriangles(),
            BU_IMMUTABLE
        );
        _camera.center(geometry->getBBox());
    }
    else
    {
        _geoBuffer = resources->getSphereGeometry();
        _camera.center(resources->getSphereBBox());
    }
}

PreviewMesh::~PreviewMesh()
{
    if(_geoBuffer && _geoBuffer != _resources->getSphereGeometry())
        delete _geoBuffer;
}

void PreviewMesh::render(float time)
{
	MaterialSystem::getSingletonRef().setBuffers();

    _resources->getBackgroundShader()->set();
    _resources->getBackgroundTexture()->set(0);

	_device->setBlendState(_resources->getBakcgroundBlendState());
	_device->setDepthStencilState(_resources->getBackgroundDepthState());
	_device->renderFullscreenQuad();

	_device->setInputLayer(VX_3D);
    _geoBuffer->set();

    _device->setDepthStencilState(_resources->getMeshDepthState());
    _resources->getMeshShader()->set();

    IConstantBuffer* cbuffer = _resources->getConstantBuffer();
	cbuffer->set();

	struct
	{
		Matrix4				viewProj;
		Vector4				eyePos;
		Vector4				lightDir;
		Vector4				lightColor;
	} buffer;

	_camera.getCamera().buildMatrices();
	buffer.viewProj=_camera.getCamera().getViewProjMatrix();
	buffer.lightDir=Vector4(1,-1,1,0);
	buffer.lightDir.normalize();
	buffer.lightColor=Vector4(0.8f,0.8f,0.8f,10.0f);
	buffer.eyePos=_camera.getCamera().getPosition();

	cbuffer->fill(&buffer);

	_material->setBase(time);

	_device->renderIndexed(_geoBuffer->getIndexCount());
}

}