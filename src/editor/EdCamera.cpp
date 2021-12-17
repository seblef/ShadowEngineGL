#include "EdCamera.h"

namespace Editor
{

EdCamera::EdCamera() :
    PreviewCamera(Core::Vector2(45.f, 45.f), 10.f)
{
}

void EdCamera::initialize(int groundWidth, int groundHeight)
{
    setTargetPosition(Core::Vector2(
        (float)(groundWidth / 2),
        (float)(groundHeight / 2)
    ));
}

void EdCamera::setTargetPosition(const Core::Vector2& pos)
{
    _camera.setTarget(Core::Vector3(pos.x, .0f, pos.y));
    update();
}

void EdCamera::translate(const Core::Vector2& translation)
{
    setTargetPosition(Core::Vector2(
        _camera.getTarget().x + translation.x,
        _camera.getTarget().z + translation.y
    ));
}

void EdCamera::scaleDistance(float f)
{
    _distance *= f;
    update();
}

}