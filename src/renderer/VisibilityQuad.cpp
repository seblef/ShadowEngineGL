
#include "VisibilityQuad.h"
#include "QuadTreeNode.h"
#include "Renderable.h"
#include "../core/CoreCommon.h"
#include "../core/BBox3.h"
#include "../core/Rect.h"

using namespace Core;


VisibilityQuad::VisibilityQuad(int min_size) : _width(65536), _height(65535)
{
	_minSize=getNearestPowerOfTwo(min_size);
	_nodes.push_back(new QuadTreeNode(0,0,0,65536,-1));
}

void VisibilityQuad::clearNodes()
{
    for(auto const& n : _nodes)
		delete n;

	_nodes.clear();
}

void VisibilityQuad::create(int width, int height)
{
	clearNodes();
	_width=width;
	_height=height;

	int size=smax(getNearestPowerOfTwo(width),getNearestPowerOfTwo(height));
	recursCreate(-1,0,0,size);
}

void VisibilityQuad::onResize(int w, int h)
{
	set<Renderable*> renderables;
    for(unsigned int i=0;i<_nodes.size();++i)
	{
        for(auto const& r : _nodes[i]->getRenderables())
			renderables.insert(r);
	}

	create(w,h);

    for(auto const &r : renderables)
	{
		r->setVisibilityID(-1);
		addRenderable(r);
	}
}

void VisibilityQuad::recursCreate(int parent, int x, int y, int size)
{
	if(x < _width && y < _height && size >= _minSize)
	{
		int id=_nodes.size();
		_nodes.push_back(new QuadTreeNode(id,x,y,size,parent));
		if(parent!=-1)
			_nodes[parent]->addChild(id);

		size=size >> 1;
		recursCreate(id,x,y,size);
		recursCreate(id,x+size,y,size);
		recursCreate(id,x,y+size,size);
		recursCreate(id,x+size,y+size,size);
	}
}

void VisibilityQuad::addRenderable(Renderable* r)
{
	if(r->getVisibilityID()!=-1)
		remRenderable(r);

	SRect rect;
	getRenderableBounds(r,rect);

	_nodes[findBestNode(rect)]->addRenderable(r);
}

void VisibilityQuad::remRenderable(Renderable* r)
{
	_nodes[r->getVisibilityID()]->remRenderable(r);
}

void VisibilityQuad::recursVisibility(const SRect& r, int id)
{
	QuadTreeNode *c,*n=_nodes[id];
	n->enqueueRenderables();

	for(int i=0;i<n->getChildrenCount();++i)
	{
		c=_nodes[n->getChild(i)];
		if(r.x1 < c->getX() + c->getSize() && r.x2 > c->getX() &&
		   r.y1 < c->getY() + c->getSize() && r.y2 > c->getY())
		   recursVisibility(r,n->getChild(i));
	}
}

void VisibilityQuad::onRenderableMoved(Renderable *r)
{
	SRect rect;
	getRenderableBounds(r,rect);

	if(!isIn(_nodes[r->getVisibilityID()],rect))
		addRenderable(r);
}

int VisibilityQuad::findBestNode(const SRect& r) const
{
	bool end=false;
	int current=0;
	QuadTreeNode* n,*c;
	while(!end)
	{
		end=true;
		n=_nodes[current];
		for(int i=0;i<n->getChildrenCount();++i)
		{
			c=_nodes[n->getChild(i)];
			if(isIn(c,r))
			{
				current=n->getChild(i);
				end=false;
			}
		}
	}

	return current;
}

int VisibilityQuad::getNearestPowerOfTwo(int n) const
{
    int r=1;
    while(n > (1 << r))
        ++r;

    return 1 << r;
}

void VisibilityQuad::getRenderableBounds(const Renderable* r, Core::SRect& rect) const
{
    const BBox3& b(r->getWorldBBox());
    rect.x1=(int)(b.getMin().x);
    rect.x2=(int)(b.getMax().x);
    rect.y1=(int)(b.getMin().z);
    rect.y2=(int)(b.getMax().z);
}

bool VisibilityQuad::isIn(const QuadTreeNode* n,const SRect& r) const
{
    return (r.x1 >= n->getX() && r.x2 <= n->getX() + n->getSize() &&
            r.y1 >= n->getY() && r.y2 <= n->getY() + n->getSize());
}
