
#include "GameEntityDynamic.h"
#include "SystemValues.h"
#include "../loguru.hpp"


GameEntityDynamic::GameEntityDynamic(EntityDynamic& t, const Matrix4& world) :
	GameEntity(t,world,true,DYNAMIC)
{
	_physic = Physic::getSingletonRef().createDynamic(*t.getPhysicGeometry(), world, t.getDensity());
	_physic->setUserData((GameObject*)this);
}

void GameEntityDynamic::update(float time)
{
	updateMatrix(((PhysicDynamic*)_physic)->getWorldMatrix());
}

void GameEntityDynamic::onHit(const Hit& h)
{
#ifdef _DEBUG
	LOG_S(1) << "Game entity has been hit; damage: " << h._damage;
#endif
	Vector3 force(h._dir * h._damage * SystemValues::getSingletonRef().getDamageToForce());
	((PhysicDynamic*)_physic)->addForce(force, h._impact, true);
}
