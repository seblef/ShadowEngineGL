
#pragma once

#include "../core/BBox3.h"
#include "../core/Vertex.h"
#include "../renderer/GeometryData.h"
#include <memory>


class Geometry
{
protected:

	int _vxCount;
	int _triCount;
	Core::BBox3 _bBox;
	std::unique_ptr<Core::Vx3D[]> _vertices;
	std::unique_ptr<unsigned short[]> _indices;
	std::unique_ptr<GeometryData> _rGeo;

public:

	Geometry(int vxCount, int triCount);

	int					getVertexCount() const			{ return _vxCount; }
	int					getTriangleCount() const		{ return _triCount; }

	Vx3D*				getVertices() const				{ return _vertices.get(); }
	Vx3D&				getVertex(int n)				{ return _vertices[n]; }
	const Vx3D&			getVertex(int n) const			{ return _vertices[n]; }

	unsigned short*		getTriangle(int n)				{ return _indices.get() + n*3; }
	unsigned short*		getTriangles() const			{ return _indices.get(); }

	void				setBBox(const BBox3& b)			{ _bBox=b; }
	const BBox3&		getBBox() const					{ return _bBox; }

	void				computeBBox();
	void				computeNormals();
	void				computeBinormalAndTangent();

	void				buildRGeometry();
	GeometryData*		getRGeometry() const			{ return _rGeo.get(); }

	void				outputVertices() const;
	void				outputTriangles() const;
};
