#pragma once

namespace Editor
{

enum ResourceType
{
    RES_MATERIAL=0,

    RES_COUNT
};

class IResource
{
private:
    ResourceType _type;

public:
    IResource(ResourceType type) : _type(type) {}
    virtual ~IResource() {}
};

}