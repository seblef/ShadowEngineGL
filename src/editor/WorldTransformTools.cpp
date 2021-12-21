#include "WorldTransformTools.h"
#include "Object.h"
#include "Selection.h"

namespace Editor
{

TranslationTool::TranslationTool() :
    SelectionTool(TOOL_TRANSLATE)
{
}

void TranslationTool::onMouseButtonPressed(MouseButton button)
{
    if(button == MB_RIGHT)
    {
        cancel();
        return;
    }

    if(_flags & TF_CTRLDOWN || Selection::getSingletonRef().getSelection().empty())
        SelectionTool::onMouseButtonPressed(button);
    else
        _buttonPressed[button] = true;

    _savedPositions.clear();
    for(auto const& obj : Selection::getSingletonRef().getSelection())
        _savedPositions[obj] = obj->getPosition();

	if(!getGroundPosition(_lastX, _lastY, _lastPos))
		_lastPos = Core::Vector3::NullVector;
	_translation = Core::Vector3::NullVector;
}

void TranslationTool::onMouseMove(int deltaX, int deltaY)
{
    SelectionTool::onMouseMove(deltaX, deltaY);

    if(!_buttonPressed[MB_LEFT])
        return;

    Core::Vector3 groundPos;
    if(!getGroundPosition(_lastX, _lastY, groundPos))
        return;

    Core::Vector3 delta(groundPos - _lastPos);
    if(_flags & TF_LOCKX)
        delta.x = .0f;
    if(_flags & TF_LOCKY)
        delta.y = .0f;
    if(_flags & TF_LOCKZ)
        delta.z = .0f;

    _translation += delta;

    for(auto const& obj : Selection::getSingletonRef().getSelection())
    {
        obj->setPosition(_savedPositions[obj] + _translation);
        if(_flags & TF_SNAP)
            snap(obj);
    }

    _lastPos = groundPos;
}

void TranslationTool::cancel()
{
    for(auto const& obj : Selection::getSingletonRef().getSelection())
        obj->setPosition(_savedPositions[obj]);
}

void TranslationTool::snap(Object* obj) const
{
    Core::Vector3 points[8];
    float xmin = 1e23f, zmin = 1e23f;
    const Core::Matrix4& world(obj->getWorldMatrix());
    obj->getLocalBBox().getPoints(points);

	for(int i=0; i<8; ++i)
	{
		points[i] *= world;
		xmin = std::min(points[i].x, xmin);
		zmin = std::min(points[i].z, zmin);
	}

    Core::Vector3 delta(
        floorf(xmin) - xmin,
        .0f,
        floorf(zmin) - zmin
    );
    obj->translate(delta);
}


RotationTool::RotationTool() :
    SelectionTool(TOOL_ROTATE)
{
}

void RotationTool::onMouseButtonPressed(MouseButton button)
{
    if(button == MB_RIGHT)
    {
        cancel();
        return;
    }

    if(_flags & TF_CTRLDOWN || Selection::getSingletonRef().getSelection().empty())
        SelectionTool::onMouseButtonPressed(button);
    else
        _buttonPressed[button] = true;

    _savedRotations.clear();
    for(auto const& obj : Selection::getSingletonRef().getSelection())
        _savedRotations[obj] = obj->getRotation();
	_rotation = Core::Vector3::NullVector;
}

void RotationTool::onMouseMove(int deltaX, int deltaY)
{
    SelectionTool::onMouseMove(deltaX, deltaY);
    if(!_buttonPressed[MB_LEFT])
        return;

	Core::Vector3 rot((float)deltaY, (float)deltaX, .0f);

    if(_flags & TF_LOCKX)
        rot.x = .0f;
    if(_flags & TF_LOCKY)
        rot.y = .0f;
    if(_flags & TF_LOCKZ)
        rot.z = .0f;

	_rotation += rot;

    for(auto const& obj : Selection::getSingletonRef().getSelection())
    {
        obj->setRotation(_savedRotations[obj] + _rotation);
        if(_flags & TF_SNAP)
            snap(obj);
    }
}

void RotationTool::cancel()
{
    for(auto const& obj : Selection::getSingletonRef().getSelection())
        obj->setRotation(_savedRotations[obj]);
}

void RotationTool::snap(Object* obj) const
{
	int rx = (int)obj->getRotation().x;
	int ry = (int)obj->getRotation().y;
	int rz = (int)obj->getRotation().z;

	rx = (rx / 45) * 45;
	ry = (ry / 45) * 45;
	rz = (rz / 45) * 45;

	Core::Vector3 rot((float)rx, (float)ry, (float)rz);
	obj->setRotation(rot);
}

}