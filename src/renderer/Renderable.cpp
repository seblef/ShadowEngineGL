
#include "Renderable.h"
#include "Renderer.h"
#include "IVisibilitySystem.h"

void Renderable::setWorldMatrix(const Matrix4& world)
{
	if(!_alwaysVisible)
		Renderer::getSingletonRef().getVisibilitySystem()->onRenderableMoved(this);

	_worldMatrix=world;
	_worldBBox.transformFromBox(_localBBox,world);
}
