
#include "BuildingTemplate.h"

BuildingTemplate::~BuildingTemplate()
{
	MeshVector::iterator m(_meshes.begin());
	for (; m != _meshes.end(); ++m)
		delete *m;

	LightVector::iterator l(_lights.begin());
	for (; l != _lights.end(); ++l)
		delete *l;
}

