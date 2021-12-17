#pragma once

#include <string>

namespace Editor
{

enum ResourceType
{
    RES_MATERIAL=0,
    RES_STATIC,
    RES_PARTICLES,
    RES_GEOMETRY,

    RES_COUNT
};

const char* const g_ResourceNames[RES_COUNT] =
{
    "Material",
    "Static",
    "Particles",
    "Geometry"
};

class IResource
{
private:
    ResourceType _type;

protected:
    std::string _name;
    bool _valid;
    bool _edited;

public:
    IResource(ResourceType type) :
        _type(type),
        _valid(true),
        _edited(false) {}
    IResource(ResourceType type, const std::string& name) :
        _type(type),
        _name(name),
        _valid(true),
        _edited(false) {}
    virtual ~IResource() {}

    const std::string& getName() const { return _name; }
    void setName(const std::string& name) { _name = name; }

    bool isValid() const { return _valid; }
    bool isEdited() const { return _edited; }
    void startEdit() { _edited = true; }
    void stopEdit() { _edited = false; }

};

}