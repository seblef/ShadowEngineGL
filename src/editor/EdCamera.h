#pragma once

#include "../core/Camera.h"

namespace Editor
{

class EdCamera
{
protected:
    Core::Camera _camera;

public:
    EdCamera();
    ~EdCamera() {}

    const Core::Camera& getCamera() const { return _camera; }
};

}