
#include "Weapon.h"
#include "GeometryLoader.h"
#include "Debris.h"
#include "ActionServer.h"
#include "SystemValues.h"
#include "../SoundLib.h"



Weapon::Weapon() : _rMaterial(0), _rMesh(0), _fireTime(0.5f), _fireSound(0),
	_debrisFactor(1), _sparkFactor(1), _firePoint(Vector3::NullVector),
	_range(10.0f), _damage(10.0f), _geometry(0),
	_flashEnable(true), _flashColor(Color::White), _flashRange(5.0f)
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

	_rMaterial=0;
	_rMesh=0;
	_fireSound = 0;
	_geometry = 0;
}

void Weapon::parseToken(const string& t, ScriptFile& sf)
{
	if (t == "geometry")				_meshName = sf.getToken();
	else if (t == "material")			_materialName = sf.getToken();
	else if (t == "fire_time")			_fireTime = stof(sf.getToken());
	else if (t == "fire_sound")			_fireSoundName = sf.getToken();
	else if (t == "debris_factor")		_debrisFactor = stof(sf.getToken());
	else if (t == "spark_factor")		_sparkFactor = stof(sf.getToken());
	else if (t == "fire_point")			sf.parseVector(_firePoint);
	else if (t == "range")				_range = stof(sf.getToken());
	else if (t == "damage")				_damage = stof(sf.getToken());
	else if (t == "no_flash")			_flashEnable = false;
	else if (t == "flash_color")		sf.parseColor(_flashColor);
	else if (t == "flash_range")		_flashRange = stof(sf.getToken());
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

