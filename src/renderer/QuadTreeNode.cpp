#include "QuadTreeNode.h"
#include "Renderable.h"

void				QuadTreeNode::addRenderable(Renderable*r)
{
    _renderables.insert(r);
    r->setVisibilityID(_id);
}

void QuadTreeNode::remRenderable(Renderable*r)
{
    _renderables.erase(r);
}

void QuadTreeNode::enqueueRenderables()
{
    for(auto const& r : _renderables)
        r->enqueue();
}