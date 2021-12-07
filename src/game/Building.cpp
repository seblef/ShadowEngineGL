
#include "Building.h"
#include "BuildingTemplate.h"
#include "BuildingMesh.h"
#include "BuildingLight.h"
#include "GameLight.h"
#include "GameMesh.h"
#include "Map.h"
#include "ResourceDB.h"

Building::Building(const BuildingTemplate& t, const Matrix4& world, Map& m) : _map(&m)
{
	BuildingMesh* bm;
	TemplateMesh* tm;
	BuildingLight* bl;
	LightCreate_t lc;
	int idx = 0;

	_objectCount = t.getLightCount() + t.getMeshCount();
	_objects = new GameObject*[_objectCount];

	for (int i = 0; i < t.getMeshCount(); ++i)
	{
		bm = t.getMesh(i);
		tm = ResourceDB::getSingletonRef().getMeshDB().getData(bm->getMeshName());
		assert(tm);

		_objects[idx]=new GameMesh(*tm, world);
		++idx;
	}

	for (int i = 0; i < t.getLightCount(); ++i)
	{
		bl = t.getLight(i);
		memcpy(&lc,&bl->getLightDesc(),sizeof(LightCreate_t));
		lc._world *= world;
		_objects[idx]=new GameLight(lc, bl->getLightType());
		++idx;
	}
}

Building::~Building()
{
//			Objects are deleted from map classes
//	for (int i = 0; i < _objectCount; ++i)
//		delete _objects[i];

	delete[] _objects;
}

void Building::onAddToMap()
{
	for (int i = 0; i < _objectCount; ++i)
		_map->addObject(_objects[i], false);
}

void Building::onRemFromMap()
{
}
