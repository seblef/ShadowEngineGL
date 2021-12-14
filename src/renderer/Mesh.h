#pragma once

class Material;
class GeometryData;

class Mesh
{
protected:

	Material*			_material;
	GeometryData*		_geometry;

public:

	Mesh(GeometryData* geo, Material* mat) : _material(mat), _geometry(geo)				{}
	virtual ~Mesh()										{}

	void				setMaterial(Material* m)		{ _material=m; }
    void                setGeometry(GeometryData* g)    { _geometry=g; }

	Material*			getMaterial() const				{ return _material; }
	GeometryData*		getGeometry() const				{ return _geometry; }
};
