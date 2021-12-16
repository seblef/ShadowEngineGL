#include "PreviewFrame.h"
#include "PreviewResources.h"
#include "imgui/imgui.h"
#include "../glmedia/GLTexture.h"
#include "../mediacommon/IDepthTexture.h"
#include "../mediacommon/IFrameBuffer.h"
#include "../mediacommon/IVideoDevice.h"


namespace Editor
{

int PreviewFrame::_id = 0;

PreviewFrame::PreviewFrame(
    IVideoDevice* device,
    PreviewResources* resources,
    int width,
    int height
) :
    _device(device),
    _resources(resources),
    _width(width),
    _height(height),
    _dragDelta(Vector2::NullVector),
    _wheel(0.f)
{
    ++_id;

    std::string backName("PreviewBackBuffer");
    backName += std::to_string(_id);
    _backBuffer = std::unique_ptr<ITexture>(_device->createTexture(
        backName,
        width,
        height,
        TEXF_A8R8G8B8,
        BU_DEFAULT,
        true
    ));
    _depthBuffer = std::unique_ptr<IDepthTexture>(
        _device->createDepthTexture(width, height, 32, false)
    );
    ITexture* back = _backBuffer.get();
    _frameBuffer = std::unique_ptr<IFrameBuffer>(_device->createFrameBuffer(
        width, height,
        1, &back,
        _depthBuffer.get()
    ));

    _camera.onResize(width, height);
}

PreviewFrame::~PreviewFrame()
{
}

void PreviewFrame::draw(float time)
{
    _frameBuffer->set();
	_device->clearDepthStencil();
    render(time);
    _device->resetRenderTargets();

    ImVec2 uv_min(0.f, 1.f);
    ImVec2 uv_max(1.f, 0.f);
    ImVec4 bg_color = ImVec4(0.f, 0.f, 0.f, 1.f);
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    ImGui::ImageButton(
        (ImTextureID)((GLTexture*)_backBuffer.get())->getGLId(),
        ImVec2((float)_width, (float)_height),
        uv_min,
        uv_max,
        0,
        bg_color,
        tint_col
    );
    processMouse();
}

void PreviewFrame::processMouse()
{
    if(ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        ImVec2 imdelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);

        _camera.rotate(Vector2(imdelta.y - _dragDelta.y, imdelta.x - _dragDelta.x));

        _dragDelta.x = imdelta.x;
        _dragDelta.y = imdelta.y;
    }
    else
        _dragDelta = Vector2::NullVector;
}

}