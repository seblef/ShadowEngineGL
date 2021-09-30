
#ifndef _MAP_H_
#define _MAP_H_

#include "Ground.h"
#include "GameObjectSet.h"
#include "GameCharacter.h"
#include "Building.h"

class Map : public GameObjectSet
{
protected:

	typedef TSet<Building>					BuildingSet;

	Ground				_ground;
	BuildingSet			_buildings;

	Vector2				_startPosition;

	IPhysicObject*		_pObject;

public:

	Map(int w, int h);
	~Map();

	void				setStartPosition(const Vector2& p)		{ _startPosition=p; }
	const Vector2&		getStartPosition() const				{ return _startPosition; }

	Ground&				getGround()						{ return _ground; }
	const Ground&		getGround() const				{ return _ground; }

	void				finalize();

	void				addBuilding(Building* b);
	void				remBuilding(Building* b);

	void				outputWakableInfo() const;
};

#endif