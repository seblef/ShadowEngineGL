
#include "Weapon.h"
#include "GameMaterial.h"
#include "GeometryLoader.h"
#include "Debris.h"
#include "ActionServer.h"
#include "SystemValues.h"
#include "../core/YAMLCore.h"
#include "../renderer/Mesh.h"
#include "../sound/SoundSystem.h"



Weapon::Weapon(const YAML::Node& node) :
	_rMaterial(0),
	_rMesh(0),
	_fireSound(0),
	_geometry(0),
	_meshName(node["mesh"].as<string>()),
	_materialName(node["material"].as<string>()),
	_fireTime(node["fire_time"].as<float>(0.5f)),
	_fireSoundName(node["fire_sound"].as<string>("")),
	_debrisFactor(node["debris_factor"].as<float>(1.f)),
	_sparkFactor(node["spark_factor"].as<float>(1.f)),
	_firePoint(node["fire_point"].as<Vector3>(Vector3::NullVector)),
	_range(node["range"].as<float>(10.0f)),
	_damage(node["damage"].as<float>(10.0f)),
	_flashEnable(node["flash_enable"].as<bool>(true)),
	_flashColor(node["flash_color"].as<Color>(Color::White)),
	_flashRange(node["flash_range"].as<float>(5.0f))
{
}

void Weapon::load()
{
	if(!_rMesh && !_rMaterial)
	{
		Geometry *_geometry=GeometryLoader::loadGeometry(_meshName);
		assert(_geometry);
		_geometry->buildRGeometry();

		_rMaterial=GameMaterial::loadMaterial(_materialName);
		assert(_rMaterial);

		_rMesh=new Mesh(_geometry->getRGeometry(),_rMaterial->getRMaterial());
	}

	if (!_fireSound && !_fireSoundName.empty())
		_fireSound = SoundSystem::getSingletonRef().loadSound(_fireSoundName);
}

void Weapon::unload()
{
	if(_rMaterial)				delete _rMaterial;
	if(_rMesh)					delete _rMesh;
	if (_fireSound)				SoundSystem::getSingletonRef().releaseSound(_fireSound);
	if (_geometry)				delete _geometry;

	_rMaterial = 0;
	_rMesh = 0;
	_fireSound = 0;
	_geometry = 0;
}

void Weapon::emitDebris(GameMaterial* mat, const Vector3& pos, const Vector3& normal)
{
	if (mat->getDebrisMesh() && _debrisFactor > 0.0f)
	{
		Debris *d = new Debris(mat->getDebrisMesh(),
			(int)((float)SystemValues::getSingletonRef().getBaseDebrisCount() * _debrisFactor) + 1,
			pos, normal,
			SystemValues::getSingletonRef().getBaseDebrisAngle() * _debrisFactor,
			SystemValues::getSingletonRef().getBaseDebrisMinVelocity() * _debrisFactor,
			SystemValues::getSingletonRef().getBaseDebrisMaxVelocity() * _debrisFactor,
			SystemValues::getSingletonRef().getBaseDebrisAngleSpeed() * _debrisFactor,
			SystemValues::getSingletonRef().getBaseDebrisSize() * _debrisFactor);
		ActionServer::getSingletonRef().addAction(d);
	}
}

