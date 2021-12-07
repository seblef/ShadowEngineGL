
#include "VisibilityAll.h"
#include "Renderable.h"

void VisibilityAll::visibility(const Core::ViewFrustum& vf, const Core::SRect& bounds)
{
    for(auto const& r : _renderables)
		r->enqueue();
}

void VisibilityAll::addRenderable(Renderable* r)
{
    _renderables.insert(r);
}

void VisibilityAll::remRenderable(Renderable* r)
{
    _renderables.erase(r);
}
