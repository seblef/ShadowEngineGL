#pragma once

#include "Renderable.h"
#include "IUpdatable.h"

class Mesh;

class MeshInstance : public Renderable, public IUpdatable
{
protected:

	const Mesh*			_model;

public:

	MeshInstance(const Mesh *model, const Matrix4& world, bool alwaysVisible);
	~MeshInstance();

	const Mesh*		getModel() const					{ return _model; }

	void			enqueue();
};
