#pragma once

#include "Weapon.h"
#include <memory>

class TemplateMesh;
class ExplosionTemplate;


namespace Particles
{
class SystemTemplate;
}

class WeaponRocket : public Weapon
{
protected:

	//				Ammo
	std::string _rocketMeshFile;
	std::string _rocketMaterialFile;
	std::string _rocketSoundFile;
	float _rocketLife;
	float _rocketVelocity;

	TemplateMesh* _rocketMesh;
	ISound* _rocketSound;

	//				Effects
	std::string _rocketExplosionName;
	ExplosionTemplate* _rocketExplosion;

	Core::Vector3 _rocketSmokePoint;
	std::unique_ptr<Particles::SystemTemplate> _rocketSmoke;

	bool _loaded;

public:

	WeaponRocket(const YAML::Node& node);
	~WeaponRocket();

	void load();
	void unload();

	TemplateMesh* getRocketMesh() const { return _rocketMesh; }
	ISound* getRocketSound() const { return _rocketSound; }
	ExplosionTemplate* getRocketExplosion() const { return _rocketExplosion; }
	Particles::SystemTemplate* getRocketSmoke() const { return _rocketSmoke.get(); }
	const Core::Vector3& getRocketSmokePoint() const { return _rocketSmokePoint; }

	float getRocketLife() const { return _rocketLife; }
	float getRocketVelocity() const { return _rocketVelocity; }

	WeaponInstance* createInstance(GameCharacter* c);
};
