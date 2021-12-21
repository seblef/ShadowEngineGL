#include "WorldTransformTools.h"
#include "Object.h"
#include "Selection.h"

namespace Editor
{

TranslationTool::TranslationTool() :
    SelectionTool(TOOL_TRANSLATE)
{
}

void TranslationTool::begin(
    int mouseX,
    int mouseY,
    int mouseWheel,
    unsigned int flags
)
{
    SelectionTool::begin(
        mouseX,
        mouseY,
        mouseWheel,
        flags
    );
    _savedPositions.clear();
}

void TranslationTool::onMouseButtonPressed(MouseButton button)
{
    SelectionTool::onMouseButtonPressed(button);
    if(button != MB_LEFT)
        return;

    _savedPositions.clear();
    for(auto const& obj : Selection::getSingletonRef().getSelection())
        _savedPositions[obj] = obj->getPosition();

	if(!getGroundPosition(_lastX, _lastY, _lastPos))
		_lastPos = Core::Vector3::NullVector;
	_translation = Core::Vector3::NullVector;
}

void TranslationTool::onMouseButtonReleased(MouseButton button)
{
    if(button == MB_LEFT)
        _savedPositions.clear();
    SelectionTool::onMouseButtonReleased(button);
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

}