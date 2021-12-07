
#include "BuildingTemplate.h"
#include "BuildingLight.h"
#include "BuildingMesh.h"


BuildingTemplate::~BuildingTemplate()
{
    for(auto const& m : _meshes)
		delete m;

    for(auto const& l : _lights)
		delete l;
}

void BuildingTemplate::addMesh(BuildingMesh* m)
{
    _meshes.push_back(m);
}

void BuildingTemplate::addLight(BuildingLight* l)
{
    _lights.push_back(l);
}

int BuildingTemplate::getMeshCount() const
{
    return _meshes.size();
}

int BuildingTemplate::getLightCount() const
{
    return _lights.size();
}

BuildingMesh* BuildingTemplate::getMesh(int n) const
{
    return _meshes[n];
}

BuildingLight* BuildingTemplate::getLight(int n) const
{
    return _lights[n];
}
