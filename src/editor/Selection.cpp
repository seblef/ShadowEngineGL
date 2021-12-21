#include "Selection.h"
#include "../core/Vector3.h"
#include "../game/Geometry.h"
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
#include "../physic/PhysicUtils.h"
#include "../loguru.hpp"


namespace Editor
{

class PhysicCallback : public physx::PxSimulationEventCallback
{
public:

    PhysicCallback()  {}

    void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) { }
    void onWake(physx::PxActor** actors, physx::PxU32 count) {}
    void onSleep(physx::PxActor** actors, physx::PxU32 count) {}
    void onContact(const physx::PxContactPairHeader& ph, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)	{}
    void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {}
	void onAdvance(const physx::PxRigidBody*const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) {};
};

static physx::PxDefaultErrorCallback g_DefaultErrorCallback;
static physx::PxDefaultAllocator g_DefaultAllocatorCallback;
static PhysicCallback g_PhysicCallback;


static physx::PxPvd* pvd;
static physx::PxPvdTransport* pvdTransport;


Selection::Selection() :
    _ground(0),
    _groundSelected(false)
{
	_foundation = PxCreateFoundation(
        PX_PHYSICS_VERSION,
        g_DefaultAllocatorCallback,
        g_DefaultErrorCallback
    );
	assert(_foundation);

    pvd = physx::PxCreatePvd(*_foundation);
    pvdTransport = physx::PxDefaultPvdFileTransportCreate("editor.pxd2");
    // _pvdTransport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
    assert(pvd->connect(*pvdTransport, physx::PxPvdInstrumentationFlag::eALL));

	_physics = PxCreatePhysics(
        PX_PHYSICS_VERSION,
        *_foundation,
        physx::PxTolerancesScale(),
        false,
        pvd
    );
	assert(_physics);

    _cooking = PxCreateCooking(
        PX_PHYSICS_VERSION,
        *_foundation,
        physx::PxCookingParams(physx::PxTolerancesScale())
    );
	assert(_cooking);

	PxInitExtensions(*_physics, pvd);

	physx::PxSceneDesc desc(_physics->getTolerancesScale());
	desc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
	desc.filterShader = physx::PxDefaultSimulationFilterShader;
	desc.gravity.x=0;
	desc.gravity.y=-9.81f;
	desc.gravity.z=0;
	desc.simulationEventCallback = &g_PhysicCallback;
	_scene = _physics->createScene(desc);
	assert(_scene);

	_defaultMaterial = _physics->createMaterial(0.5,0.5,0.5);
}

Selection::~Selection()
{
    _defaultMaterial->release();
    _scene->release();
    _cooking->release();
    _physics->release();

    PxCloseExtensions();
    _foundation->release();
}

physx::PxRigidDynamic* Selection::createActor() const
{
    physx::PxTransform t(physx::PxVec3(0));
    if(!t.isValid())
        t.q.normalize();
    physx::PxRigidDynamic* actor = _physics->createRigidDynamic(t);
    actor->setRigidBodyFlags(physx::PxRigidBodyFlag::eKINEMATIC);
    return actor;
}

void Selection::createGroundActor(int width, int height)
{
    float halfWidth = 0.5f * (float)width;
    float halfHeight = 0.5f * (float)height;

	//			Hack
	Core::Matrix4 world;
	world.createTranslate(-halfWidth, -0.5f, -halfHeight);
	PMAKETRANSFORM(t, world);

    _ground = _physics->createRigidDynamic(t);
    _ground->setRigidBodyFlags(physx::PxRigidBodyFlag::eKINEMATIC);

	physx::PxShape *shape = _physics->createShape(
        physx::PxBoxGeometry(
		    (float)width + 0.5f,
            1.0f,
            (float)height + 0.5f
        ),
        *_defaultMaterial
    );
    _ground->attachShape(*shape);
    shape->release();

    _scene->addActor(*_ground);
}

void Selection::releaseGroundActor()
{
    if(_ground)
    {
        _scene->removeActor(*_ground);
        _ground->release();
        _ground = 0;
    }
}

physx::PxTriangleMeshGeometry* Selection::createMeshGeometry(const Geometry& geo) const
{
    physx::PxTriangleMeshDesc desc;
    physx::PxDefaultMemoryOutputStream stream;

    desc.points.count = geo.getVertexCount();
    desc.points.data = geo.getVertices();
    desc.points.stride = Core::g_VertexSize[VX_3D];
    desc.triangles.count = geo.getTriangleCount();
    desc.triangles.data = (const void*)geo.getTriangles();
    desc.triangles.stride = 3 * sizeof(unsigned short);
    desc.flags = physx::PxMeshFlag::e16_BIT_INDICES;

    _cooking->cookTriangleMesh(desc, stream);

    physx::PxDefaultMemoryInputData input(stream.getData(), stream.getSize());
    physx::PxTriangleMesh* mesh = _physics->createTriangleMesh(input);

    return new physx::PxTriangleMeshGeometry(mesh);
}

void Selection::releaseMeshGeometry(physx::PxTriangleMeshGeometry* geo) const
{
    delete geo;
}

physx::PxRigidDynamic* Selection::createSphereActor(float radius) const
{
    physx::PxRigidDynamic* actor = createActor();
    physx::PxShape *shape = _physics->createShape(
        physx::PxSphereGeometry(radius),
        *_defaultMaterial
    );
    actor->attachShape(*shape);
    shape->release();
    return actor;
}

physx::PxRigidDynamic* Selection::createBoxActor(const Core::Vector3& size) const
{
    physx::PxRigidDynamic* actor = createActor();
    physx::PxShape *shape = _physics->createShape(
        physx::PxBoxGeometry(size.x*.5f, size.y*.5f, size.z*.5f),
        *_defaultMaterial
    );
    actor->attachShape(*shape);
    shape->release();
    return actor;
}

physx::PxRigidDynamic* Selection::createMeshActor(const physx::PxTriangleMeshGeometry& geo) const
{
    physx::PxRigidDynamic* actor = createActor();
    physx::PxShape *shape = _physics->createShape(
        geo,
        *_defaultMaterial
    );
    actor->attachShape(*shape);
    shape->release();
    return actor;
}

void Selection::clearSelection()
{
    _selection.clear();
    _groundSelected = false;
}

void Selection::clearSavedSelection()
{
    _savedSelection.clear();
}

Object* Selection::rayTrace(
    const Core::Vector3& origin,
    const Core::Vector3& direction,
    bool& ground
) const
{
    ground = false;
    physx::PxVec3 dir(direction.x, direction.y, direction.z);
    physx::PxVec3 orig(origin.x, origin.y, origin.z);
    dir.normalize();

    physx::PxRaycastBuffer hit;

    if(_scene->raycast(orig, dir, 1e6, hit, physx::PxHitFlag::ePOSITION))
    {
        if(hit.block.actor == _ground)
            ground = true;
        return (Object*)hit.block.actor->userData;
    }
    return 0;
}

Object* Selection::select(
    const Core::Vector3& origin,
    const Core::Vector3& direction,
    bool addToSelection,
    bool& ground
)
{
    Object* object = rayTrace(origin, direction, ground);
    _groundSelected = ground;

    if(!addToSelection)
        _selection.clear();
    if(object)
        _selection.insert(object);
    return object;
}

void Selection::select(Object* object, bool addToSelection)
{
    if(!addToSelection)
        clearSelection();
    _selection.insert(object);
}

void Selection::refresh()
{
    _scene->fetchResults(true);
    _scene->simulate(.02f);
}

void Selection::saveSelection()
{
    clearSavedSelection();
    for(auto const& obj : _selection)
        _savedSelection.insert(obj);
}

void Selection::restoreSelection()
{
    clearSelection();
    for(auto const& obj : _savedSelection)
        _selection.insert(obj);
}

void Selection::addActorToScene(physx::PxRigidDynamic& actor)
{
    _scene->addActor(actor);
}

void Selection::remActorFromScene(physx::PxRigidDynamic& actor)
{
    _scene->removeActor(actor);
}

}