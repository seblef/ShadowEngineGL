#pragma once

#include "PhysicObject.h"
#include <PxPhysicsAPI.h>


using namespace physx;
using namespace Core;


class PhysicActorController : public PhysicObject, public PxUserControllerHitReport
{
protected:

	PxController*				_controller;
	Vector3						_offset;
	Vector3						_position;

	PxGeometry*					_actorGeo;

public:

	PhysicActorController(const PhysicActorCtrlCreate_t& c,
		PxPhysics* p, PxScene* s, PxControllerManager* m);
	~PhysicActorController();


	const Vector3&				move(const Vector3& m, float elpasedTime);
	
	void						setPosition(const Vector3& p);
	const Vector3&				getPosition() const				{ return _position; }

	void						addToScene();
	void						remFromScene();

	void						onShapeHit(const PxControllerShapeHit& hit);
	void						onControllerHit(const PxControllersHit& hit);
	void						onObstacleHit(const PxControllerObstacleHit& hit);

	float						getDistanceFromPoint(const Vector3& p, Vector3& hitPoint);
};
