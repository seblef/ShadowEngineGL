#include "Resources.h"
#include "EdMaterial.h"

namespace Editor
{

bool Resources::exists(ResourceType type, const std::string& name) const
{
    auto const& res = _resources[type].find(name);
    return res != _resources[type].end();
}

IResource *Resources::get(ResourceType type, const std::string& name) const
{
    const auto res = _resources[type].find(name);
    if(res == _resources[type].end())
        return 0;
    else
        return res->second.get();
}

void Resources::add(ResourceType type, IResource* res, const std::string& name)
{
    _resources[type].insert(std::make_pair(name, res));
}

void Resources::drop(ResourceType type, const std::string& name)
{
    const auto res = _resources[type].find(name);
    if(res != _resources[type].end())
        _resources[type].erase(res);
}

IResource* Resources::load(ResourceType type, const std::string& name)
{
    IResource* res = get(type, name);
    if(res)
        return res;
    
    if(type == RES_MATERIAL)
    {
        std::unique_ptr<EdMaterial> mat(new EdMaterial(name));
        if(!mat->isValid())
            return 0;
        res = mat.get();
        _resources[RES_MATERIAL][name] = std::move(mat);
    }
    return res;
}

}