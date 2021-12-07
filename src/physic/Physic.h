#pragma once

#include "../core/Matrix4.h"
#include "../core/TSingleton.h"
#include "../core/TDynamicTab.h"
#include <PxPhysicsAPI.h>
#include <set>

class IPhysicObject;
class PhysicGeometry;
class PhysicMaterial;
class PhysicDynamic;
class PhysicEvent;
class PhysicQuery;
class RayCastInfos;

struct PhysicGeometryCreate_t;
struct PhysicActorCtrlCreate_t;

using namespace Core;
using namespace physx;
using namespace std;

class Physic : public TSingleton<Physic>, public PxSimulationEventCallback
{
protected:

	PxPhysics*						_physic;
	PxScene*						_scene;
	PxCooking*						_cooking;
	PxFoundation*					_foundation;
	PxControllerManager*			_ctrlMgr;
	PxPvd*							_pvd;
	PxPvdTransport*					_pvdTransport;

	PhysicMaterial*					_defaultMat;

	float							_updateTime;
	float							_time;

	typedef set<PhysicDynamic*>		DynamicSet;
	DynamicSet						_activeDynamics;

	TDynamicTab<PhysicEvent>		_events;

	unsigned int					_queryID;

public:

	Physic(float updateTime, const Vector3& gravity, bool debugMode=false);
	~Physic();

	PhysicGeometry*					createGeometry(const PhysicGeometryCreate_t& c) const;

	IPhysicObject*					createGround(int width, int height, PhysicMaterial* mat=0) const;
	IPhysicObject*					createStaticScene(const PhysicGeometry& geo, const Matrix4& world, PhysicMaterial* mat=0) const;
	IPhysicObject*					createKinematicScene(const PhysicGeometry& geo, const Matrix4& world, PhysicMaterial* mat=0) const;
	IPhysicObject*					createActorController(PhysicActorCtrlCreate_t& c) const;
	IPhysicObject*					createDynamic(const PhysicGeometry& geo, const Matrix4& world, float density, PhysicMaterial* mat = 0) const;
	IPhysicObject*					createTrigger(const PhysicGeometry& geo, const Matrix4& world) const;
	IPhysicObject*					createAmmo(const PhysicGeometry& geo, const Matrix4& world) const;

	void							update(float time);
	const PhysicEvent*				getEvents(int& count) const;
	void							clearEvents();

	IPhysicObject*					rayCast(RayCastInfos& rc) const;
	IPhysicObject*					rayCastImpact(RayCastInfos& rc) const;
	bool							testRay(const RayCastInfos& rc) const;

	void							query(PhysicQuery& q);

	//			Callback functions
	void							onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)		{}
	void							onWake(PxActor** actors, PxU32 count);
	void							onSleep(PxActor** actors, PxU32 count);
	void							onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
	void							onTrigger(PxTriggerPair* pairs, PxU32 count);
	void 							onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) {};

	//			Internal functions
	void							remDynamicFromActiveList(PhysicDynamic* d);
	void							postEvent(const PhysicEvent& e);

};
