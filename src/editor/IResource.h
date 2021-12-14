#pragma once

#include <string>

namespace Editor
{

enum ResourceType
{
    RES_MATERIAL=0,
    RES_GEOMETRY,

    RES_COUNT
};

const char* const g_ResourceNames[RES_COUNT] =
{
    "Material",
    "Geometry"
};

class IResource
{
private:
    ResourceType _type;

protected:
    std::string _name;

public:
    IResource(ResourceType type) : _type(type) {}
    IResource(ResourceType type, const std::string& name) : _type(type), _name(name) {}
    virtual ~IResource() {}

    const std::string& getName() const { return _name; }
    void setName(const std::string& name) { _name = name; }
};

}