#pragma once

#include "PhysicObject.h"
#include "PhysicGeometry.h"

class PhysicStaticScene : public PhysicObject
{
public:

	PhysicStaticScene(const PhysicGeometry& geo,
		const Matrix4& world, PhysicMaterial *m,
		PxPhysics* p, PxScene* s);
	~PhysicStaticScene();
};
