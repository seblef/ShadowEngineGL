#pragma once

#include "EdSolid.h"
#include "IResource.h"

namespace Editor
{

class EdStaticTemplate : public EdSolidTemplate, public IResource
{
public:
    EdStaticTemplate(const std::string& name);
    EdStaticTemplate(
        const std::string& name,
        const YAML::Node& node
    );
};

class StaticObject : public SolidObject
{
public:
    StaticObject(EdStaticTemplate* stat);
    StaticObject(const StaticObject& stat);
};

}