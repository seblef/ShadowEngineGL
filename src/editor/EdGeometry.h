#pragma once

#include "IResource.h"
#include <memory>

class Geometry;

namespace Editor
{

class EdGeometry : public IResource
{
protected:
    std::unique_ptr<Geometry> _geometry;
    bool _valid;

public:
    EdGeometry(const std::string& filename);

    Geometry* getGeometry() const { return _geometry.get(); }

    bool isValid() const { return _valid; }
};

}