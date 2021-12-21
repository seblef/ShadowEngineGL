#pragma once

#include "../core/TSingleton.h"
#include "../mediacommon/IVideoDevice.h"
#include <list>
#include <map>


namespace Editor
{

#define MAX_DRAWER_LINES        256

class EdCamera;
class Object;
class PreviewResources;

class Drawer : public Core::TSingleton<Drawer>
{
protected:
    IVideoDevice* _device;
    PreviewResources* _resources;

    ITexture* _backBuffer;
    IFrameBuffer* _frameBuffer;

    IShader* _linesShader;
    IVertexBuffer* _linesVBuffer;
    Core::VxBase _linesVerts[MAX_DRAWER_LINES*2];
    int _linesVertCount;
    RenderState _linesRenderState;

    IShader* _solidShader;
    IGeometryBuffer* _solidGeoBuffer;
    DepthStencilState _solidDepthState;
    RenderState _solidRenderState;
    std::map<unsigned int, ITexture*> _objectsTextures;
    std::map<unsigned int, std::list<Object*>> _objects;

    Core::Color _color;
    DepthStencilState _linesDepthState;
    DepthStencilState _depthOverState;
    BlendState _noBlendState;
    BlendState _addBlendState;
    SamplerState _trilinearSamplerState;
    SamplerState _copySamplerState;

    void executeLines();
    void loadData();
    void cleanUp();
    void drawSolids();
    void drawSelection();

public:
    Drawer(IVideoDevice* device, PreviewResources* resources);
    ~Drawer();

    void onNewMap();

    void draw(EdCamera& camera);
    void onResize(int width, int height);

    void beginLines(bool noZBuffer=false);
    void endLines();

    void setWorldMatrix(const Core::Matrix4& world);
    void setColor(const Core::Color& color);

    void addObject(Object* obj);
    void remObject(Object* obj);

    void drawLine(const Core::Vector3& v1, const Core::Vector3& v2);
    void drawDot(const Core::Vector3& pos, float size);
    void drawCircle(const Core::Vector3& center, const Core::Vector3& axis, float size);
    void drawBox(const Core::Vector3& v1, const Vector3& v2);
    void drawRect(
        const Core::Vector3& v1,
        const Core::Vector3& v2,
        const Core::Vector3& v3,
        const Core::Vector3& v4
    );
    void drawGroundGrid();
};

}