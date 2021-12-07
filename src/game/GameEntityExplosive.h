
#pragma once

#include "GameEntityDynamic.h"


class EntityExplosive;

class GameEntityExplosive : public GameEntityDynamic
{
protected:

	float				_damage;
	bool				_hasExploded;

public:

	GameEntityExplosive(EntityExplosive& e, const Matrix4& world);

	void				onHit(const Hit& h);
};
