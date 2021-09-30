
#ifndef _MESH_H_
#define _MESH_H_

#include "Material.h"
#include "GeometryData.h"

class Mesh
{
protected:

	Material*			_material;
	GeometryData*		_geometry;

public:

	Mesh(GeometryData* geo, Material* mat) : _material(mat), _geometry(geo)				{}
	virtual ~Mesh()										{}

	void				setMaterial(Material* m)		{ _material=m; }

	Material*			getMaterial() const				{ return _material; }
	GeometryData*		getGeometry() const				{ return _geometry; }
};

#endif