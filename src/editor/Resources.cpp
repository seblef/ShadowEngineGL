#include "Resources.h"

namespace Editor
{

Resources::~Resources()
{
    for(int i=0; i<RES_COUNT; ++i)
        for(auto const& r : _resources[i])
            delete r.second;    
}

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
        return res->second;
}

void Resources::add(ResourceType type, IResource* res, const std::string& name)
{
    _resources[type].insert(std::make_pair(name, res));
}

void Resources::drop(ResourceType type, const std::string& name)
{
    const auto res = _resources[type].find(name);
    if(res != _resources[type].end())
    {
        delete res->second;
        _resources[type].erase(res);
    }
}

}