
#include "GameEntityExplosive.h"
#include "EntityExplosive.h"
#include "ActionServer.h"
#include "Explosion.h"
#include "Hit.h"
#include "../loguru.hpp"

GameEntityExplosive::GameEntityExplosive(EntityExplosive& e, const Matrix4& world) : GameEntityDynamic(e, world), _damage(0),
	_hasExploded(false)
{
}

void GameEntityExplosive::onHit(const Hit& h)
{
	GameEntityDynamic::onHit(h);

	if (_hasExploded)
		return;

	_damage += h._damage;
	EntityExplosive* e = (EntityExplosive*)_template;

	if (_damage >= e->getDamageThreshold())
	{
#ifdef _DEBUG
		LOG_S(1) << "Explosive entity max damaged reached, exploding it";
#endif
		_damage = 0;
        Vector3 pos;
        pos.x=_world(3,0);
        pos.y=_world(3,1);
        pos.z=_world(3,2);
		ActionServer::getSingletonRef().addDelayedAction(new Explosion(*e->getExplosion(), pos),0.1f);
		_hasExploded = true;
	}
}
