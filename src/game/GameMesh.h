
#pragma once

#include "GameObject.h"


class TemplateMesh;
class IPhysicObject;
class MeshInstance;


class GameMesh : public GameObject
{
protected:

	MeshInstance*				_rInstance;
	IPhysicObject*				_physic;
	const TemplateMesh*			_template;

public:

	GameMesh(const TemplateMesh& t, const Matrix4& world, bool alwaysVisible=false);
	~GameMesh();

	void						onAddToScene();
	void						onRemFromScene();
	void						onUpdateMatrix(const Matrix4& new_m)	{}

	const TemplateMesh*			getTemplate() const					{ return _template; }
};
