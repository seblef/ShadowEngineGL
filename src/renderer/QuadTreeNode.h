#pragma once

#include <set>

class Renderable;

using namespace std;

class QuadTreeNode
{
protected:

	int					_id;
	int					_x,_y,_size;
	int					_parent;
	int					_childrenCount;
	int					_children[4];

	typedef set<Renderable*>		RenderableSet;
	RenderableSet		_renderables;

public:

	QuadTreeNode(int id, int x, int y, int size, int parent) : _id(id), _x(x), _y(y),
        _size(size), _parent(parent), _childrenCount(0)			{}
	~QuadTreeNode()							{}

	int					getID() const				{ return _id; };
	int					getX() const				{ return _x; }
	int					getY() const				{ return _y; }
	int					getSize() const				{ return _size; }

	int					getChildrenCount() const	{ return _childrenCount; }
	int					getChild(int n) const		{ return _children[n]; }
	
	void				addChild(int c)				{ _children[_childrenCount++]=c; }
	
	void				addRenderable(Renderable*r);
	void				remRenderable(Renderable*r);
	void				enqueueRenderables();

	const set<Renderable*>&	getRenderables() const	{ return _renderables; }
};
