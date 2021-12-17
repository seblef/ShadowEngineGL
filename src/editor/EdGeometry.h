#pragma once

#include "IResource.h"
#include "../game/Geometry.h"
#include <memory>

class Geometry;

namespace Editor
{

class EdGeometry : public IResource
{
protected:
    std::unique_ptr<Geometry> _geometry;

public:
    EdGeometry(const std::string& filename);

    Geometry* getGeometry() const { return _geometry.get(); }
};

}