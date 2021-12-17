#include "Resources.h"
#include "EdGeometry.h"
#include "EdMaterial.h"
#include "EdParticles.h"

namespace Editor
{

bool Resources::exists(ResourceType type, const std::string& name) const
{
    auto const& res = _resources[type].find(name);
    return res != _resources[type].end();
}

IResource *Resources::get(
    ResourceType type,
    const std::string& name,
    bool loadIfNotExists
)
{
    const auto res = _resources[type].find(name);
    if(res == _resources[type].end())
    {
        if(loadIfNotExists)
            return load(type, name);
        else
            return 0;
    }
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
    if(exists(type, name))
        return get(type, name);
    IResource* res = 0;
    
    if(type == RES_MATERIAL)
    {
        std::unique_ptr<EdMaterial> mat(new EdMaterial(name));
        if(!mat->isValid())
            return 0;
        res = mat.get();
        _resources[RES_MATERIAL][name] = std::move(mat);
    }
    else if(type == RES_PARTICLES)
    {
        std::unique_ptr<EdParticles> particles(new EdParticles(name));
        if(!particles->isValid())
            return 0;
        res = particles.get();
        _resources[RES_PARTICLES][name] = std::move(particles);
    }
    else if(type == RES_GEOMETRY)
    {
        std::unique_ptr<EdGeometry> geometry(new EdGeometry(name));
        if(!geometry->isValid())
            return 0;
        res = geometry.get();
        _resources[RES_GEOMETRY][name] = std::move(geometry);
    }
    return res;
}

}