#include "PreviewMesh.h"
#include "PreviewResources.h"
#include "imgui/imgui.h"
#include "../mediacommon/IConstantBuffer.h"
#include "../mediacommon/IDepthTexture.h"
#include "../mediacommon/IFrameBuffer.h"
#include "../mediacommon/IGeometryBuffer.h"
#include "../mediacommon/IShader.h"
#include "../mediacommon/ITexture.h"
#include "../mediacommon/IVideoDevice.h"
#include "../game/Geometry.h"
#include "../glmedia/GLTexture.h"
#include "../renderer/Material.h"
#include "../renderer/MaterialSystem.h"

namespace Editor
{


int PreviewMesh::_id = 0;


PreviewMesh::PreviewMesh(
    Geometry* geometry,
    Material* material,
    IVideoDevice* device,
    PreviewResources* resources,
    int width,
    int height
) : 
    _width(width),
    _height(height),
    _material(material),
    _device(device),
    _resources(resources),
    _backBuffer(0),
    _depthBuffer(0),
    _frameBuffer(0),
    _geoBuffer(0),
    _dragDelta(Vector2::NullVector),
    _wheel(0.f)
{
    ++_id;

    std::string backName("PreviewBackBuffer");
    backName += std::to_string(_id);
    _backBuffer = _device->createTexture(
        backName,
        width,
        height,
        TEXF_A8R8G8B8,
        BU_DEFAULT,
        true
    );
    _depthBuffer = _device->createDepthTexture(width, height, 32, false);
    _frameBuffer = _device->createFrameBuffer(
        width, height,
        1, &_backBuffer,
        _depthBuffer
    );

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

    _camera.onResize(width, height);
}

PreviewMesh::~PreviewMesh()
{
    if(_frameBuffer)
        delete _frameBuffer;
    if(_depthBuffer)
        delete _depthBuffer;
    if(_backBuffer)
        delete _backBuffer;

    if(_geoBuffer && _geoBuffer != _resources->getSphereGeometry())
        delete _geoBuffer;
}

void PreviewMesh::draw()
{
    render();

    ImVec2 uv_min(0.f, 0.f);
    ImVec2 uv_max(1.f, 1.f);
    ImVec4 bg_color = ImVec4(0.f, 0.f, 0.f, 1.f);
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    ImGui::ImageButton(
        (ImTextureID)((GLTexture*)_backBuffer)->getGLId(),
        ImVec2((float)_width, (float)_height),
        uv_min,
        uv_max,
        0,
        bg_color,
        tint_col
    );
    processMouse();
}

void PreviewMesh::render()
{
    _frameBuffer->set();

	MaterialSystem::getSingletonRef().setBuffers();
	_device->clearDepthStencil();

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

	_material->setBase(0.f);

	_device->renderIndexed(_geoBuffer->getIndexCount());

    _device->resetRenderTargets();
}

void PreviewMesh::processMouse()
{
    if(ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        ImVec2 imdelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);

        _camera.rotate(Vector2(_dragDelta.y - imdelta.y, imdelta.x - _dragDelta.x));

        _dragDelta.x = imdelta.x;
        _dragDelta.y = imdelta.y;
    }
    else
        _dragDelta = Vector2::NullVector;
}

}