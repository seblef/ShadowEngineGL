#pragma once

#include "IResource.h"
#include "../core/TSingleton.h"
#include <map>
#include <memory>
#include <string>


namespace Editor
{

class EdMaterial;


class Resources : public Core::TSingleton<Resources>
{
protected:
    std::map<std::string, std::unique_ptr<IResource>> _resources[RES_COUNT];

public:
    Resources() {}

    bool exists(ResourceType type, const std::string& name) const;
    IResource *get(ResourceType type, const std::string& name) const;
    void add(ResourceType type, IResource* res, const std::string& name);
    IResource* load(ResourceType type, const std::string& name);
    void drop(ResourceType type, const std::string& name);

    const std::map<std::string, std::unique_ptr<IResource>>& getAll(ResourceType type) const { return _resources[type]; }
};

}