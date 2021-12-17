#pragma once

#include "PreviewCamera.h"

namespace Editor
{

class EdCamera : public PreviewCamera
{
public:
    EdCamera();
    ~EdCamera() {}

    void initialize(int groundWidth, int groundHeight);
    void setTargetPosition(const Core::Vector2& target);
    void translate(const Core::Vector2& translation);
    void scaleDistance(float f);
};

}
