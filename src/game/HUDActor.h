
#pragma once

#include "../core/Vector3.h"

using namespace Core;

class HUDActor
{
protected:

	float			_actorHeight;
	float			_maxLife;
	float			_currentLife;
	Vector3			_position;

public:

    HUDActor(float maxLife, float actorHeight) : _actorHeight(actorHeight),_maxLife(maxLife),
		_currentLife(maxLife), _position(Vector3::NullVector)	{}
	~HUDActor()													{}

	void			updatePosition(const Vector3& pos)			{ _position = pos; }
	void			updateLife(float life)						{ _currentLife = life; }

	const Vector3&	getPosition() const							{ return _position; }
	float			getMaxLife() const							{ return _maxLife; }
	float			getCurrentLife() const						{ return _currentLife; }
	float			getActorHeight() const						{ return _actorHeight; }
};
