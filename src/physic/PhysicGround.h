#pragma once

#include "PhysicObject.h"

class PhysicGround : public PhysicObject
{
public:

	PhysicGround(PxPhysics* physic, PxScene* scene, PhysicMaterial* m,
		int height, int width);
	~PhysicGround();
};
