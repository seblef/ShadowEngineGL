#include "Drawer.h"
#include "EdCamera.h"
#include "Object.h"
#include "PreviewResources.h"
#include "Selection.h"
#include "../core/Quaternion.h"
#include "../game/GeometryLoader.h"
#include "../mediacommon/IDepthTexture.h"
#include "../mediacommon/IFrameBuffer.h"
#include "../mediacommon/IGeometryBuffer.h"
#include "../mediacommon/IShader.h"
#include "../mediacommon/ITexture.h"
#include "../mediacommon/IVertexBuffer.h"
#include "../renderer/Renderer.h"
#include "../loguru.hpp"

namespace Editor
{

Drawer::Drawer(IVideoDevice* device, PreviewResources* resources) :
    _device(device),
    _resources(resources),
    _backBuffer(0),
    _frameBuffer(0),
    _solidGeoBuffer(0),
    _linesVertCount(0),
    _color(Core::Color::White)
{
    _linesShader = device->createShader("Editor/Lines");
    _linesVBuffer = device->createVertexBuffer(MAX_DRAWER_LINES*2, VX_BASE, BU_DYNAMIC);
    _linesRenderState = device->createRenderState(CULL_NONE);
    _linesDepthState = device->createDepthStencilState(true, false, COMP_LESSEQUAL);

    _solidShader = device->createShader("Editor/Objects");
    _solidDepthState = device->createDepthStencilState(true, true, COMP_LESSEQUAL);
    _solidRenderState = device->createRenderState(CULL_BACK);

    _depthOverState = device->createDepthStencilState(false, false, COMP_ALWAYS);
	_noBlendState = device->createBlendState(false, BLEND_ONE, BLEND_ZERO);
	_addBlendState = device->createBlendState(true, BLEND_ONE, BLEND_ONE);
    _trilinearSamplerState = device->createSamplerState(FILTER_TRILINEAR);
    _copySamplerState = device->createSamplerState(FILTER_POINT, ADDRESS_CLAMP, ADDRESS_CLAMP);

    onResize(
        _device->getResWidth(),
        _device->getResHeight()
    );

    _objects[OBJ_LIGHT] = std::list<Object*>();

    loadData();
}

Drawer::~Drawer()
{
    _linesShader->remRef();
    _solidShader->remRef();
    delete _linesVBuffer;
    if(_solidGeoBuffer)
        delete _solidGeoBuffer;

    _device->destroyBlendState(_noBlendState);
    _device->destroyBlendState(_addBlendState);
    _device->destroyDepthStencilState(_depthOverState);
    _device->destroySamplerState(_trilinearSamplerState);

    _device->destroyDepthStencilState(_linesDepthState);
    _device->destroyRenderState(_linesRenderState);

    _device->destroyDepthStencilState(_solidDepthState);
    _device->destroyRenderState(_solidRenderState);

    for(auto const& tex : _objectsTextures)
        tex.second->remRef();

    cleanUp();
}

void Drawer::loadData()
{
    std::unique_ptr<Geometry> boxGeo(GeometryLoader::loadGeometry("Geometry/Editor/Box.GEO"));
    _solidGeoBuffer = _device->createGeometryBuffer(
        boxGeo->getVertexCount(),
        VX_3D,
        boxGeo->getVertices(),
        boxGeo->getTriangleCount() * 3,
        IDX_16BITS,
        boxGeo->getTriangles(),
        BU_IMMUTABLE
    );

    _objectsTextures[OBJ_LIGHT] = _device->createTexture("Textures/Editor/Light.dds");
}

void Drawer::cleanUp()
{
    if(_backBuffer)
        _backBuffer->remRef();
    if(_frameBuffer)
        delete _frameBuffer;
}

void Drawer::onNewMap()
{
    for(auto& objs : _objects)
        objs.second.clear();
}

void Drawer::draw(EdCamera& camera)
{
    Renderer::getSingletonRef().update(.0f, &camera.getCamera());

    _frameBuffer->set();

    drawSolids();
    
    beginLines();
    drawGroundGrid();
    endLines();

    drawSelection();

    _device->resetRenderTargets();
    _device->setBlendState(_noBlendState);
    _device->setSamplerState(0, _copySamplerState);
    _backBuffer->set(0);
    _resources->getBackgroundShader()->set();
    _device->renderFullscreenQuad();
}

void Drawer::onResize(int width, int height)
{
    if(width != _device->getResWidth() || height != _device->getResHeight())
    {
        Renderer::getSingletonRef().onResize(width, height);
        _device->onResize(width, height);
    }

    IDepthTexture* depthBuffer = Renderer::getSingletonRef().getGBuffer().getDepthBuffer();
    _backBuffer =  _device->createTexture(
        "draw_backbuffer",
        width,
        height,
        TEXF_A8R8G8B8,
        BU_DEFAULT,
        true
    );
    _frameBuffer = _device->createFrameBuffer(
        width, height,
        1, &_backBuffer,
        depthBuffer
    );
    Renderer::getSingletonRef().setFrameBuffer(_frameBuffer, false);
}

void Drawer::executeLines()
{
	if(_linesVertCount)
	{
		_linesVBuffer->fill(_linesVerts, _linesVertCount);
		_device->render(_linesVertCount);
		_linesVertCount = 0;
	}
}

void Drawer::beginLines(bool noZBuffer)
{
	_linesShader->set();
	_linesVBuffer->set();
	_device->setBlendState(_noBlendState);
	_device->setRenderState(_linesRenderState);
	_device->setDepthStencilState(noZBuffer ? _depthOverState : _linesDepthState);
	_device->setInputLayer(VX_BASE);
	_device->setPrimitiveType(PT_LINELIST);

    Renderer::getSingletonRef().setSceneInfos();
	Renderer::getSingletonRef().setObjectTransformation(Core::Matrix4::Identity);
}

void Drawer::endLines()
{
    executeLines();
    _device->setRenderState(_solidRenderState);
}

void Drawer::setWorldMatrix(const Core::Matrix4& world)
{
	executeLines();
	Renderer::getSingletonRef().setObjectTransformation(world);
}

void Drawer::setColor(const Core::Color& color)
{
    _color = color;
}

void Drawer::drawLine(const Core::Vector3& v1, const Core::Vector3& v2)
{
	_linesVerts[_linesVertCount].pos = v1;
	_linesVerts[_linesVertCount].color = _color;

	_linesVerts[_linesVertCount+1].pos = v2;
	_linesVerts[_linesVertCount + 1].color = _color;

	_linesVertCount += 2;
	if(_linesVertCount >= MAX_DRAWER_LINES*2)
		executeLines();
}

void Drawer::drawDot(const Core::Vector3& v, float size)
{
	drawLine(Core::Vector3(v.x - size,v.y,v.z),Vector3(v.x + size,v.y,v.z));
	drawLine(Core::Vector3(v.x,v.y - size,v.z),Vector3(v.x,v.y + size,v.z));
	drawLine(Core::Vector3(v.x,v.y,v.z - size),Vector3(v.x,v.y,v.z + size));
}

void Drawer::drawCircle(const Core::Vector3& center, const Core::Vector3& axis, float size)
{
	const float step = 2.f * M_PI / 36.0f;
	float angle = 0.0f;
	for(int i=0; i<36; ++i, angle+=step)
	{
		Core::Quaternion q1(axis, angle);
		Core::Quaternion q2(axis, angle + step);

		drawLine(
            center + (q1 * Core::Vector3::XAxisVector) * size,
            center + (q2 * Core::Vector3::XAxisVector) * size
        );
	}
}

void Drawer::drawBox(const Core::Vector3& v1, const Vector3& v2)
{
	drawLine(v1,Core::Vector3(v2.x,v1.y,v1.z));
	drawLine(Core::Vector3(v2.x,v1.y,v1.z),Core::Vector3(v2.x,v2.y,v1.z));
	drawLine(Core::Vector3(v2.x,v2.y,v1.z),Core::Vector3(v1.x,v2.y,v1.z));
	drawLine(Core::Vector3(v1.x,v2.y,v1.z),v1);

	drawLine(Core::Vector3(v1.x,v1.y,v2.z),Core::Vector3(v2.x,v1.y,v2.z));
	drawLine(Core::Vector3(v2.x,v1.y,v2.z),Core::Vector3(v2.x,v2.y,v2.z));
	drawLine(Core::Vector3(v2.x,v2.y,v2.z),Core::Vector3(v1.x,v2.y,v2.z));
	drawLine(Core::Vector3(v1.x,v2.y,v2.z),Core::Vector3(v1.x,v1.y,v2.z));

	drawLine(Core::Vector3(v1.x,v1.y,v2.z),Core::Vector3(v1.x,v1.y,v1.z));
	drawLine(Core::Vector3(v2.x,v1.y,v2.z),Core::Vector3(v2.x,v1.y,v1.z));
	drawLine(Core::Vector3(v2.x,v2.y,v2.z),Core::Vector3(v2.x,v2.y,v1.z));
	drawLine(Core::Vector3(v1.x,v2.y,v2.z),Core::Vector3(v1.x,v2.y,v1.z));
}

void Drawer::drawRect(
    const Core::Vector3& v1,
    const Core::Vector3& v2,
    const Core::Vector3& v3,
    const Core::Vector3& v4
)
{
	drawLine(v1,v2);
	drawLine(v2,v3);
	drawLine(v3,v4);
	drawLine(v4,v1);
}

void Drawer::drawGroundGrid()
{
    setColor(Core::Color::Red);
    setWorldMatrix(Core::Matrix4::Identity);

	const Core::SRect &r(Renderer::getSingletonRef().getVisibilityBounds());
	float xmin = (float)r.x1;
	float xmax = (float)r.x2;
	float ymin = (float)r.y1;
	float ymax = (float)r.y2;

	float x = xmin, y = ymin;
	while(x <= xmax)
	{
        drawLine(
            Core::Vector3(x, .0f, ymin),
            Core::Vector3(x, .0f, ymax)
        );
        x += 1.f;
	}

	while(y <= ymax)
	{
        drawLine(
            Core::Vector3(xmin, .0f, y),
            Core::Vector3(xmax, .0f, y)
        );
        y += 1.f;
	}
}

void Drawer::addObject(Object* obj)
{
    _objects[obj->getType()].push_back(obj);
}

void Drawer::remObject(Object* obj)
{
    _objects[obj->getType()].remove(obj);
}

void Drawer::drawSolids()
{
    const ObjectType types[] = {
        OBJ_LIGHT
    };
    const unsigned int typeCount = 1;

    Renderer::getSingletonRef().setSceneInfos();
    _device->setBlendState(_noBlendState);
    _device->setRenderState(_solidRenderState);
    _device->setDepthStencilState(_solidDepthState);
    _device->setSamplerState(0, _trilinearSamplerState);
	_device->setInputLayer(VX_3D);
	_device->setPrimitiveType(PT_TRIANGLELIST);
    _solidShader->set();
    _solidGeoBuffer->set();

    Core::Matrix4 world(Matrix4::Identity);

    for(unsigned int i=0;i<typeCount;++i)
    {
        _objectsTextures[types[i]]->set(0);
        for(auto const& obj : _objects[types[i]])
        {
            world = obj->getWorldMatrix();
            Renderer::getSingletonRef().setObjectTransformation(world);
            _device->renderIndexed(_solidGeoBuffer->getIndexCount());
        }
    }
}

void Drawer::drawSelection()
{
    beginLines(true);
    for(auto const& obj : Selection::getSingletonRef().getSelection())
        obj->drawSelected(*this);
    endLines();
}

}