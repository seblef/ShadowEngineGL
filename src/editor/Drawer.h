#pragma once

#include "../core/TSingleton.h"
#include "../mediacommon/IVideoDevice.h"

namespace Editor
{

#define MAX_DRAWER_LINES        256

class EdCamera;
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

    Core::Color _color;
    DepthStencilState _depthState;
    DepthStencilState _depthOverState;
    BlendState _noBlendState;

    void executeLines();
    void cleanUp();

public:
    Drawer(IVideoDevice* device, PreviewResources* resources);
    ~Drawer();

    void draw(EdCamera& camera);
    void onResize(int width, int height);

    void beginLines(bool noZBuffer=false);
    void endLines();

    void setWorldMatrix(const Core::Matrix4& world);
    void setColor(const Core::Color& color);

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