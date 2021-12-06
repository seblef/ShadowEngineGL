
#include "WeaponRocket.h"
#include "WeaponRocketInstance.h"
#include "GameSystem.h"
#include "../SoundLib.h"
#include "../core/YAMLCore.h"


WeaponRocket::WeaponRocket(const YAML::Node& node) :
	Weapon(node),
	_rocketLife(2.0f),
	_rocketVelocity(10.0f),
	_rocketSmokePoint(Vector3::NullVector),
	_rocketMesh(0),
	_rocketExplosion(0),
	_rocketSmoke(0), 
	_loaded(false),
	_rocketSound(0)
{
	YAML::Node rock(node["rocket"]);
	_rocketMeshFile = rock["mesh"].as<string>();
	_rocketMaterialFile = rock["material"].as<string>();
	_rocketLife = rock["life"].as<float>(2.f);
	_rocketVelocity = rock["velocity"].as<float>(10.f);
	_rocketExplosionName = rock["explosion"].as<string>("");
	_rocketSmokePoint = rock["smoke_point"].as<Vector3>(Vector3::NullVector);
	_rocketSoundFile = rock["sound"].as<string>("");

    YAML::Node smoke = rock["smoke"];
	if(smoke)
		_rocketSmoke = new ParticleSystemTemplate(smoke);
}

WeaponRocket::~WeaponRocket()
{
	unload();
}

void WeaponRocket::load()
{
	if (_loaded)
		return;

	if (!_rocketMeshFile.empty() && !_rocketMaterialFile.empty())
		_rocketMesh = TemplateMesh::loadTemplate(_rocketMeshFile, _rocketMaterialFile, 0, PSHAPE_CAPSULE);

	if (!_rocketExplosionName.empty())
	{
		_rocketExplosion = (ExplosionTemplate*)GameSystem::getSingletonRef().getEffectDB().getData(_rocketExplosionName);
		assert(_rocketExplosion);
        _rocketExplosion->load();
	}

	if (!_rocketSoundFile.empty())
		_rocketSound = SoundSystem::getSingletonRef().loadSound(_rocketSoundFile);

	Weapon::load();

	_loaded = true;
}

void WeaponRocket::unload()
{
	if (!_loaded)
		return;

	if (_rocketMesh)			delete _rocketMesh;
	if (_rocketSound)			SoundSystem::getSingletonRef().releaseSound(_rocketSound);

	_rocketMesh = 0;
	_rocketExplosion = 0;
	_rocketSound = 0;

	Weapon::unload();

	_loaded = false;
}

WeaponInstance* WeaponRocket::createInstance(GameCharacter *c)
{
	return new WeaponRocketInstance(this, c);
}
