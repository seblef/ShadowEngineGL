#include "PreviewCamera.h"
#include "../core/Quaternion.h"

namespace Editor
{

PreviewCamera::PreviewCamera(
    const Core::Vector2& rotation,
    float distance
) :
    _rotation(rotation),
    _distance(distance),
    _camera(
        -Core::Vector3::ZAxisVector,
        Core::Vector3::NullVector,
        .5f,
        100.f,
        Core::Vector2(10.f, 10.f),
        90.f
    )
{
    update();
}

void PreviewCamera::rotate(const Core::Vector2& rotation)
{
    _rotation += rotation;
    update();
}

void PreviewCamera::center(const Core::BBox3& bbox)
{
    _camera.setTarget(bbox.getCenter());
    _distance = bbox.getSize().getLength() * 0.5f;
    update();
}

void PreviewCamera::onResize(int width, int height)
{
    _camera.setCenter(
        Core::Vector2(
            (float)(width / 2),
            (float)(height / 2)
        )
    );
}

void PreviewCamera::update()
{
    Core::Quaternion roty(
        Core::Vector3::YAxisVector,
        _rotation.y * M_PI / 180.f
    );
    Core::Vector3 x(Core::Vector3::XAxisVector * roty);
    Core::Vector3 z(Core::Vector3::ZAxisVector * roty);

    Core::Quaternion rotx(x, _rotation.x * M_PI / 180.f);
    z = z * rotx;

    _camera.setPosition(_camera.getTarget() - (z * _distance));
    _camera.buildMatrices();
}

}