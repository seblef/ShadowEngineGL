#pragma once

#include "IVisibilitySystem.h"
#include <set>

class VisibilityAll : public IVisibilitySystem
{
protected:

	typedef std::set<Renderable*>		RenderableSet;
	RenderableSet					_renderables;

public:

	VisibilityAll()					{}
	~VisibilityAll()				{}

	void							create(int width, int height)				{}
	void							onResize(int w, int h)						{}

	void							addRenderable(Renderable* r);
	void							remRenderable(Renderable* r);

	void							onRenderableMoved(Renderable *r)			{}

	void							visibility(const Core::ViewFrustum& vf, const Core::SRect& bounds);
};
