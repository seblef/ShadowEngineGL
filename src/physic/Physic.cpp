
#include "Physic.h"
#include "PhysicDynamic.h"
#include "PhysicGeometry.h"
#include "PhysicGround.h"
#include "PhysicStaticScene.h"
#include "PhysicKinematicScene.h"
#include "PhysicActorController.h"
#include "PhysicTrigger.h"
#include "PhysicAmmo.h"
#include "PhysicUtils.h"
#include "PhysicQuery.h"
#include "PhysicEvent.h"
#include "RayCastInfos.h"
#include "../loguru.hpp"


static PxDefaultErrorCallback			g_DefaultErrorCallback;
static PxDefaultAllocator				g_DefaultAllocatorCallback;


PxFilterFlags PhysicFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// let triggers through
	if ((PxFilterObjectIsTrigger(attributes0) && (filterData1.word0 & ACTORGROUP_ACTOR)) || 
		(PxFilterObjectIsTrigger(attributes1) && (filterData0.word0 & ACTORGROUP_ACTOR)))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
 
	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlag::eDEFAULT;
}


Physic::Physic(float updateTime, const Vector3& gravity, bool debugMode) : _updateTime(updateTime),
	_time(0), _pvd(0), _pvdTransport(0), _events(128), _queryID(0)
{
	_foundation=PxCreateFoundation(PX_PHYSICS_VERSION, g_DefaultAllocatorCallback, g_DefaultErrorCallback);
	assert(_foundation);

	if(debugMode)
	{
		LOG_S(INFO) << "Running PhysX in debug mode";
		_pvd = PxCreatePvd(*_foundation);
		_pvdTransport = PxDefaultPvdFileTransportCreate("physx.pxd2");
		assert(_pvd->connect(*_pvdTransport,PxPvdInstrumentationFlag::eALL));
	}

	_physic=PxCreatePhysics(PX_PHYSICS_VERSION,*_foundation,PxTolerancesScale(),false,_pvd);
	assert(_physic);

    _cooking=PxCreateCooking(PX_PHYSICS_VERSION,*_foundation,PxCookingParams(PxTolerancesScale()));
	assert(_cooking);

	PxInitExtensions(*_physic, _pvd);

	PxSceneDesc desc(_physic->getTolerancesScale());
	desc.cpuDispatcher=PxDefaultCpuDispatcherCreate(1);
	desc.filterShader = PhysicFilterShader;
	desc.gravity.x=gravity.x;
	desc.gravity.y=gravity.y;
	desc.gravity.z=gravity.z;
	desc.flags|=PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS | PxSceneFlag::eENABLE_KINEMATIC_PAIRS;
	desc.simulationEventCallback=this;
	_scene=_physic->createScene(desc);
	_scene->setSimulationEventCallback(this);
	assert(_scene);

    _ctrlMgr=PxCreateControllerManager(*_scene);
	assert(_ctrlMgr);

	_defaultMat = new PhysicMaterial(0.5f, 0.5f, 0.5f, _physic);
}

Physic::~Physic()
{
	delete _defaultMat;

	_scene->release();
	_cooking->release();
	_physic->release();

	if(_pvd)			_pvd->release();
	if(_pvdTransport)	_pvdTransport->release();

	PxCloseExtensions();

	_foundation->release();
}

PhysicGeometry* Physic::createGeometry(const PhysicGeometryCreate_t& c) const
{
    return new PhysicGeometry(c,_physic,_cooking);
}

void Physic::update(float time)
{
	_time+=time;
//	_events.clear();

	if(_time >= _updateTime)
	{
		_scene->fetchResults(true);
		while(_time >= _updateTime)
		{
			_scene->simulate(_updateTime);
			_time-=_updateTime;
		}
	}

    for(auto const& d : _activeDynamics)
		d->updateWorldMatrix();
}

const PhysicEvent* Physic::getEvents(int& count) const
{
	count = _events.getCount();
	return _events.getBuffer();
}

void Physic::clearEvents()
{
    _events.clear();
}

void Physic::postEvent(const PhysicEvent& e)
{
    _events.add(e);
}

bool Physic::testRay(const RayCastInfos& rc) const
{
	PxRaycastBuffer hit;
	PxSceneQueryFilterData filterData;
	filterData.data.word0 = rc._flags;
    PxVec3 d(rc._dir.x,rc._dir.y,rc._dir.z);
    d.normalize();

    return _scene->raycast(PXVEC_C(rc._origin), d ,
		rc._range, hit, PxHitFlags(PxHitFlag::eDEFAULT), filterData);
}

void Physic::query(PhysicQuery& q)
{
	++_queryID;

    PxTransform pose(PXVEC_C(q._center));
    assert(pose.isValid());

    PxQueryFilterData filter;
	filter.data.word0 = q._groupMask;
    filter.flags=PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC;

	PxOverlapHit buffer[20];
	PxMemZero(&buffer, sizeof(buffer));
    PxOverlapBuffer hitBuffer(buffer, 20);
    bool res=false;

	switch (q._shape)
	{
	case PSHAPE_BOX:
        res = _scene->overlap(PxBoxGeometry(PXVEC_C(q._size)), pose, hitBuffer, filter);
		break;

	case PSHAPE_SPHERE:
        res = _scene->overlap(PxSphereGeometry(q._size.x), pose, hitBuffer, filter);
		break;

	case PSHAPE_CAPSULE:
        res = _scene->overlap(PxCapsuleGeometry(q._size.x, q._size.y), pose, hitBuffer, filter);
		break;
	}

	q._objectCount = 0;

    if(res)
    {
        IPhysicObject* o;

        for (int i = 0; i < hitBuffer.getNbAnyHits(); ++i)
        {
            o = (IPhysicObject*)hitBuffer.getAnyHit(i).actor->userData;
            if (o && o->getQueryID() != _queryID)
            {
                q._objects[q._objectCount] = o;
                ++q._objectCount;
                o->setQueryID(_queryID);
            }
        }
    }
}

IPhysicObject* Physic::rayCast(RayCastInfos& rc) const
{
    PxVec3 d(rc._dir.x,rc._dir.y,rc._dir.z);
    d.normalize();
    PxQueryFilterData filterData;
    filterData.data.word0 = rc._flags;
    PxRaycastBuffer hit;

    if(_scene->raycast(PXVEC_C(rc._origin),d,rc._range,hit,PxHitFlag::ePOSITION,filterData))
    {
        return (IPhysicObject*)hit.block.actor->userData;
    }
    else
        return 0;
}

IPhysicObject* Physic::rayCastImpact(RayCastInfos& rc) const
{
    PxRaycastBuffer hit;
    PxQueryFilterData filterData;
	filterData.data.word0 = rc._flags;

    PxVec3 d(rc._dir.x,rc._dir.y,rc._dir.z);
    d.normalize();

    if (_scene->raycast(PXVEC_C(rc._origin), d, rc._range, hit,
        PxSceneQueryFlag::ePOSITION | PxSceneQueryFlag::eNORMAL, filterData))
	{
        rc._impact.x = hit.block.position.x;
        rc._impact.y = hit.block.position.y;
        rc._impact.z = hit.block.position.z;
        rc._normal.x = hit.block.normal.x;
        rc._normal.y = hit.block.normal.y;
        rc._normal.z = hit.block.normal.z;
        return (IPhysicObject*)hit.block.actor->userData;
	}
	else
		return 0;
}

IPhysicObject* Physic::createGround(int w, int h, PhysicMaterial *m) const
{
	return new PhysicGround(_physic, _scene, m ? m : _defaultMat, w, h);
}

IPhysicObject* Physic::createStaticScene(const PhysicGeometry& geo, const Matrix4& world,
	PhysicMaterial* m) const
{
	return new PhysicStaticScene(geo, world, m ? m : _defaultMat, _physic, _scene);
}

IPhysicObject* Physic::createKinematicScene(const PhysicGeometry& geo, const Matrix4& world,
	PhysicMaterial* m) const
{
	return new PhysicKinematicScene(geo, world, m ? m : _defaultMat, _physic, _scene);
}

IPhysicObject* Physic::createActorController(PhysicActorCtrlCreate_t& c) const
{
	if (c._material == 0)
		c._material = _defaultMat;

	return new PhysicActorController(c, _physic, _scene, _ctrlMgr);
}

IPhysicObject* Physic::createDynamic(const PhysicGeometry& geo, const Matrix4& world,
	float density, PhysicMaterial* m) const
{
	return new PhysicDynamic(geo, world, m ? m : _defaultMat, _physic, _scene, density);
}

IPhysicObject* Physic::createTrigger(const PhysicGeometry& geo, const Matrix4& world) const
{
	return new PhysicTrigger(geo, world, _defaultMat, _physic, _scene);
}

IPhysicObject* Physic::createAmmo(const PhysicGeometry& geo, const Matrix4& world) const
{
	return new PhysicAmmo(geo, world, _defaultMat, _physic, _scene);
}

void Physic::onWake(PxActor** actors, PxU32 count)
{
	IPhysicObject* o;
	for (PxU32 i = 0; i < count; ++i)
	{
		o = (IPhysicObject*)actors[i]->userData;
		if (o && o->getObjectType() == POBJ_DYNAMIC)
		{
			_activeDynamics.insert((PhysicDynamic*)o);
			_events.add(PhysicEvent(PhysicEvent::WAKEUP, o, 0));
		}
	}
}

void Physic::onSleep(PxActor** actors, PxU32 count)
{
	IPhysicObject* o;
	for (PxU32 i = 0; i < count; ++i)
	{
		o = (IPhysicObject*)actors[i]->userData;
		if (o && o->getObjectType() == POBJ_DYNAMIC)
		{
			_activeDynamics.erase((PhysicDynamic*)o);
			_events.add(PhysicEvent(PhysicEvent::SLEEP, o, 0));
		}
	}
}

void Physic::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	IPhysicObject* a1, *a2, *actor,*target;
	PhysicObjectType t1, t2;

	for (PxU32 i = 0; i < nbPairs; ++i)
	{
		const PxContactPair& cp = pairs[i];
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
            a1 = (IPhysicObject*)pairHeader.actors[0]->userData;
            a2 = (IPhysicObject*)pairHeader.actors[1]->userData;

			t1 = a1->getObjectType();
			t2 = a2->getObjectType();

			if (t1 == POBJ_AMMO)
			{
				actor = a1;
				target = a2;
			}
			else if (t2 == POBJ_AMMO)
			{
				actor = a2;
				target = a1;
			}
			else
				continue;

			_events.add(PhysicEvent(PhysicEvent::CONTACT, actor, target));
			break;
		}
	}
}

void Physic::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (PxU32 i = 0; i < count; ++i)
	{
		if (pairs[i].status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			IPhysicObject *t = 0;
			IPhysicObject* a = 0;

            t = (IPhysicObject*)pairs[i].triggerActor->userData;
            a = (IPhysicObject*)pairs[i].otherActor->userData;

			assert(a && t && a->getObjectType() == POBJ_ACTOR && t->getObjectType() == POBJ_TRIGGER);
			_events.add(PhysicEvent(PhysicEvent::TRIGGER, a, t));
		}
	}
}

void Physic::remDynamicFromActiveList(PhysicDynamic* d)
{
	_activeDynamics.erase(d);
}
