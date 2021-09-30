
#include "Map.h"
#include "Building.h"
#include "BBoxToRect.h"
#include "ResourceDB.h"
#include "../loguru.hpp"


Map::Map(int w, int h) : _ground(w,h), _startPosition(5,5)
{
    Renderer::getSingletonRef().getVisibilitySystem()->create(w,h);

	PhysicGeometryCreate_t c;
	c._box.setMin(Vector3(0, -1, 0));
	c._box.setMax(Vector3((float)w, 0, (float)h));
	c._shape = PSHAPE_BOX;
	PhysicGeometry* geo = Physic::getSingletonRef().createGeometry(c);
	_pObject = Physic::getSingletonRef().createStaticScene(*geo, Matrix4::Identity);
	_pObject->addToScene();
}

Map::~Map()
{
	_pObject->remFromScene();
	delete _pObject;

	ResourceDB::getSingletonRef().clear();
}

void Map::finalize()
{
	_ground.finalize();
}

void Map::addBuilding(Building* b)
{
	_buildings.getSet().insert(b);
	b->onAddToMap();
}

void Map::remBuilding(Building* b)
{
	_buildings.getSet().erase(b);
	b->onRemFromMap();
}

void Map::outputWakableInfo() const
{
	LOG_SCOPE_F(1, "Map wakable info:");
	for(int y=0;y<_ground.getHeight();++y)
	{
		for(int x=0;x<_ground.getWidth();++x)
			LOG_S(1) << (_ground.isCaseWakable(x,y) ? "0 " : "1 ");
	}
}
