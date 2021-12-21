#pragma once

#include "../core/TSingleton.h"
#include <set>

class Geometry;

namespace Core
{
class Vector3;
}

namespace physx
{

class PxPhysics;
class PxScene;
class PxCooking;
class PxFoundation;
class PxMaterial;
class PxRigidDynamic;
class PxTriangleMeshGeometry;

}

namespace Editor
{

class Object;

class Selection : public Core::TSingleton<Selection>
{
protected:
    physx::PxPhysics* _physics;
    physx::PxScene* _scene;
    physx::PxCooking* _cooking;
    physx::PxFoundation* _foundation;

    physx::PxMaterial* _defaultMaterial;
    physx::PxRigidDynamic* _ground;
    
    std::set<Object*> _selection;
    std::set<Object*> _savedSelection;
    bool _groundSelected;

    physx::PxRigidDynamic* createActor() const;

public:
    Selection();
    ~Selection();

    physx::PxScene* getScene() const { return _scene; }

    void createGroundActor(int width, int height);
    void releaseGroundActor();

    physx::PxTriangleMeshGeometry* createMeshGeometry(const Geometry& geo) const;
    void releaseMeshGeometry(physx::PxTriangleMeshGeometry* geo) const;

    physx::PxRigidDynamic* createSphereActor(float radius) const;
    physx::PxRigidDynamic* createBoxActor(const Core::Vector3& size) const;
    physx::PxRigidDynamic* createMeshActor(const physx::PxTriangleMeshGeometry& geo) const;

    void clearSelection();
    const std::set<Object*>& getSelection() const { return _selection; }
    bool isGroundSelected() const { return _groundSelected; }

    Object* rayTrace(
        const Core::Vector3& origin,
        const Core::Vector3& direction,
        bool& ground
    ) const;
    Object* select(
        const Core::Vector3& origin,
        const Core::Vector3& direction,
        bool addToSelection,
        bool& ground
    );
    void select(Object* object, bool addToSelection);

    void refresh();

    void saveSelection();
    void restoreSelection();
    void clearSavedSelection();

    void addActorToScene(physx::PxRigidDynamic& actor);
    void remActorFromScene(physx::PxRigidDynamic& actor);
};

}