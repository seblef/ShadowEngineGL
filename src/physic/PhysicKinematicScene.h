#pragma once

#include "PhysicObject.h"

class PhysicKinematicScene : public PhysicObject
{
protected:

	Matrix4				_world;

public:

	PhysicKinematicScene(const PhysicGeometry& geo,
		const Matrix4& world, PhysicMaterial *m,
		PxPhysics* p, PxScene* s);
	~PhysicKinematicScene();

	void				setWorldMatrix(const Matrix4& world);
	const Matrix4&		getWorldMatrix()			{ return _world; }
};
