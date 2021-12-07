#pragma once

class Renderable;

namespace Core
{
    class ViewFrustum;
    class SRect;
}

class IVisibilitySystem
{
public:

	IVisibilitySystem()				{}
	virtual ~IVisibilitySystem()	{}

	virtual void				create(int width, int height)=0;
	virtual void				onResize(int w, int h)=0;

	virtual void				addRenderable(Renderable *r)=0;
	virtual void				remRenderable(Renderable *r)=0;

	virtual void				onRenderableMoved(Renderable* r)=0;

	virtual void				visibility(const Core::ViewFrustum& vf, const Core::SRect& bounds)=0;
};
