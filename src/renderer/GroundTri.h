#pragma once

#include "../core/Vertex.h"

using namespace Core;

class GroundTri
{
protected:

	Vx3D		_verts[3];
	int			_matID;

public:

	GroundTri() : _matID(0)					{}
	GroundTri(int x, int y, int side, int mat_id)
	{
		create(x,y,side,mat_id);
	}
	GroundTri(const GroundTri& t) : _matID(t._matID)
	{
		for(int i=0;i<3;++i)
			_verts[i]=t._verts[i];
	}

	GroundTri&			operator=(const GroundTri& t)
	{
		_matID=t._matID;
		for(int i=0;i<3;++i)
			_verts[i]=t._verts[i];
		return *this;
	}

	bool				operator < (const GroundTri& t) const				{ return _matID < t._matID; }

	int					getMatID() const				{ return _matID; }
	void				setMatID(int m)					{ _matID=m; }

	const Vx3D&			getVertice(int n) const			{ return _verts[n]; }
	const Vx3D*			getVertices() const				{ return _verts; }

	void				create(int x, int y, int side, int mat_id);
};
