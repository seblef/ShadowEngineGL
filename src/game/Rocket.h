
#pragma once

#include "Ammo.h"
#include "../sound/SoundVirtualSource.h"

class WeaponRocket;
class GameCharacter;
class MeshInstance;
class Light;
class RParticles;

class Rocket : public Ammo
{
protected:

	WeaponRocket*			_weapon;
	GameCharacter*			_owner;

	Matrix4					_world;
	Matrix4					_smokeWorld;

	Vector3					_velocity;
	float					_life;

	MeshInstance*			_rocketMesh;
	Light*					_rocketLight;
	IPhysicObject*			_rocketPhysic;
	RParticles*				_rocketSmoke;

	SoundVirtualSource		_rocketSource;

	void					explode();
	void					cleanUp();

public:

	Rocket(WeaponRocket* weapon, GameCharacter* owner,
		const Matrix4& originalWorld, const Vector3& direction);
	~Rocket();

	void					update(float time);

	void					onContact(IPhysicObject* other);
};
