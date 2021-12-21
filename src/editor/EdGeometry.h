#pragma once

#include "IResource.h"
#include "../game/Geometry.h"
#include <memory>

class Geometry;

namespace physx
{

class PxTriangleMeshGeometry;

}

namespace Editor
{

class EdGeometry : public IResource
{
protected:
    std::unique_ptr<Geometry> _geometry;
    physx::PxTriangleMeshGeometry* _selectionGeo;

public:
    EdGeometry(const std::string& filename);
    ~EdGeometry();

    Geometry* getGeometry() const { return _geometry.get(); }
    physx::PxTriangleMeshGeometry* getSelectionGeometry() const { return _selectionGeo; }
};

}