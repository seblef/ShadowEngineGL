#pragma once

#include "IVisibilitySystem.h"
#include <vector>

class QuadTreeNode;

using namespace Core;

class VisibilityQuad : public IVisibilitySystem
{
protected:

	int						_minSize;
	int						_width, _height;
	typedef std::vector<QuadTreeNode*> NodeVector;
	NodeVector				_nodes;

	int						getNearestPowerOfTwo(int n) const;

	void					clearNodes();
	void					recursVisibility(const Core::SRect& r, int id);
	void					recursCreate(int parent_id, int x, int y, int size);

	void					getRenderableBounds(const Renderable* r, Core::SRect& rect) const;
	int						findBestNode(const SRect& r) const;
	bool					isIn(const QuadTreeNode* n,const SRect& r) const;

public:

	VisibilityQuad(int min_size);
	~VisibilityQuad()					{ clearNodes(); }

	void					create(int w, int h);
	void					onResize(int w, int h);

	void					addRenderable(Renderable* r);
	void					remRenderable(Renderable* r);
	void					visibility(const ViewFrustum& vf, const SRect& bounds)				{ recursVisibility(bounds,0); }

	void					onRenderableMoved(Renderable *r);
};
