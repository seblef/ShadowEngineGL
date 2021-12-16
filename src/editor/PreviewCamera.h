#pragma once

#include "../core/Camera.h"

namespace Editor
{

class PreviewCamera
{
protected:
    Core::Camera _camera;
    Core::Vector2 _rotation;
    float _distance;

    void update();

public:

    PreviewCamera(
        const Core::Vector2& rotation=Core::Vector2::NullVector,
        float distance=10.f
    );

    void rotate(const Core::Vector2& rotation);
    void center(const Core::BBox3& bbox);

    void onResize(int width, int height);

    Core::Camera& getCamera() { return _camera; }
    const Core::Vector2& getRotation() const { return _rotation; }
    float getDistance() const { return _distance; }
};

}