
#include "ExplosionTemplate.h"
#include "ObjectFlags.h"
#include "TemplateMesh.h"
#include "../core/YAMLCore.h"
#include "../particles/ParticlePointEmitterTemplate.h"
#include "../particles/Material.h"
#include "../particles/ParticleSystemTemplate.h"
#include "../sound/SoundSystem.h"



const float ExplosionMinMass = 0.2f;
const float ExplosionMaxMass = 0.5f;
const float ExplosionMinFriction = 0.01f;
const float ExplosionMaxFriction = 0.02f;
const float ExplosionMinSizeX = 0.2f;
const float ExplosionMinSizeY = 0.2f;
const float ExplosionMaxSizeX = 0.5f;
const float ExplosionMaxSizeY = 0.5f;

const float ExplosionSmokeMinLife = 2.4f;
const float ExplosionSmokeMaxLife = 3.0f;
const float ExplosionSmokeMinMass = 0.5f;
const float ExplosionSmokeMaxMass = 0.5f;
const float ExplosionSmokeMinFriction = 0.01f;
const float ExplosionSmokeMaxFriction = 0.02f;
const Vector3 ExplosionSmokeMinVelocity(-1, 0.8f, -1);
const Vector3 ExplosionSmokeMaxVelocity(1, 1, 1);
const Color ExplosionSmokeMinColor(0.8f, 0.8f, 0.8f, 1.0f);
const float ExplosionSmokeMinStartSize = 0.1f;
const float ExplosionSmokeMaxStartSize = 0.2f;
const float ExplosionSmokeMinEndSize = 0.3f;
const float ExplosionSmokeMaxEndSize = 0.5f;
const float ExplosionSmokeEmissionRate = 50.0f;
const int ExplosionSmokeMaxParticles = 200;


ExplosionTemplate::ExplosionTemplate(const YAML::Node& node) :
	Effect(EXPLOSION),
	_size(node["size"].as<float>(1.f)),
	_life(node["life"].as<float>(1.f)),
	_color(node["color"].as<Core::Color>(Color::White)),
	_explosionColorBlend(node["color_blend"].as<bool>(false)),
	_explosionSoundFile(node["sound"].as<string>("")),
	_particleCount(node["particle_count"].as<int>(5)),
	_innerRadius(node["inner_radius"].as<float>(1.f)),
	_outerRadius(node["outer_radius"].as<float>(2.f)),
	_innerDamage(node["damage"].as<float>(50.f)),
	_hitEnvironment(node["hit_environment"].as<bool>(true)),
	_explosionTextureDir(node["texture_folder"].as<string>("")),
	_debrisSize(.01f),
	_debrisMinVelocity(1.f),
	_debrisMaxVelocity(10.f),
	_debrisMaxAngle((float)M_PI_4),
	_debrisMaxAngleSpeed((float)M_PI),
	_debrisCount(5),
	_debrisMesh(0),
	_smokeColorBlend(false),
	_smokeDensity(1.f),
	_smokeLife(.5f),
	_particles(0),
	_loaded(false), 
	_explosionEmitter(-1),
	_smokeEmitter(-1),
	_explosionSound(0)
{
    YAML::Node smoke = node["smoke"];
	if(smoke)
	{
        _smokeTexture = smoke["texture"].as<string>();
		_smokeDensity = smoke["density"].as<float>(1.f);
		_smokeLife = smoke["life"].as<float>(.5f);
		_smokeColorBlend = smoke["color_blend"].as<bool>(false);
	}
    YAML::Node debris = node["debris"];
	if(debris)
	{
		_debrisSize = debris["size"].as<float>(.01f);
		_debrisMinVelocity = debris["min_velocity"].as<float>(1.f);
		_debrisMaxVelocity = debris["max_velocity"].as<float>(10.f);
		_debrisMaxAngle = debris["max_angle"].as<float>((float)M_PI_4);
		_debrisMaxAngleSpeed = debris["max_angle_speed"].as<float>((float)M_PI);
		_debrisCount = debris["count"].as<int>(5);
		_debrisMeshFile = debris["mesh"].as<string>("");
		_debrisMaterial = debris["material"].as<string>("");
	}
}

ExplosionTemplate::~ExplosionTemplate()
{
	unload();
}

void ExplosionTemplate::load()
{
	if (_loaded)		return;

	int pcount = 0;
	ParticleEmitterTemplate* emitters[2] = { 0, 0 };

    if (!_explosionTextureDir.empty())
	{
		BlendMode src = BLEND_SRCALPHA;
		BlendMode dest = BLEND_INVSRCALPHA;

		if (_explosionColorBlend)
		{
			src = BLEND_SRCCOLOR;
			dest = BLEND_INVSRCCOLOR;
		}

		Particles::Material *m = new Particles::Material(_explosionTextureDir,true,src,dest);
		ParticleEmitterParams minp(_life * 0.5f, ExplosionMinMass, ExplosionMinFriction,
			_color, _color,
			Vector3::NullVector,
			Vector3(ExplosionMinSizeX, ExplosionMinSizeY, 0),
			Vector3(_size, _size, _size));
		ParticleEmitterParams maxp(_life, ExplosionMaxMass, ExplosionMaxFriction,
			_color, _color, Vector3::NullVector,
			Vector3(ExplosionMaxSizeX, ExplosionMaxSizeY, 0),
			Vector3(_size, _size, _size));

		emitters[0] = new ParticlePointEmitterTemplate("quad", m, minp, maxp, 30,
			Vector3::NullVector, 0);
		_explosionEmitter = 0;
		++pcount;
	}

	if (!_smokeTexture.empty())
	{
		BlendMode src = BLEND_SRCALPHA;
		BlendMode dest = BLEND_INVSRCALPHA;

		if (_smokeColorBlend)
		{
			src = BLEND_SRCCOLOR;
			dest = BLEND_INVSRCCOLOR;
		}

		Particles::Material* m = new Particles::Material(_smokeTexture,false,src, dest);
		ParticleEmitterParams minp(ExplosionSmokeMinLife,
			ExplosionSmokeMinMass,
			ExplosionSmokeMinFriction,
			ExplosionSmokeMinColor,
			Color::Black,
			ExplosionSmokeMinVelocity,
			Vector3(ExplosionSmokeMinStartSize, ExplosionSmokeMinStartSize, ExplosionSmokeMinStartSize),
			Vector3(ExplosionSmokeMinEndSize, ExplosionSmokeMinEndSize, ExplosionSmokeMinEndSize));
		ParticleEmitterParams maxp(ExplosionSmokeMaxLife,
			ExplosionSmokeMaxMass,
			ExplosionSmokeMaxFriction,
			Color::White,
			Color::Black,
			ExplosionSmokeMaxVelocity,
			Vector3(ExplosionSmokeMaxStartSize, ExplosionSmokeMaxStartSize, ExplosionSmokeMaxStartSize),
			Vector3(ExplosionSmokeMaxEndSize, ExplosionSmokeMaxEndSize, ExplosionSmokeMaxEndSize));

		emitters[pcount] = new ParticlePointEmitterTemplate("quad", m, minp, maxp,
			(unsigned int)(_smokeDensity * (float)ExplosionSmokeMaxParticles),
			Vector3::NullVector, ExplosionSmokeEmissionRate * _smokeDensity);
		_smokeEmitter = pcount;
		++pcount;
	}

	_particles = new ParticleSystemTemplate(pcount, emitters);

    if (!_debrisMeshFile.empty() && !_debrisMaterial.empty())
		_debrisMesh = TemplateMesh::loadTemplate(_debrisMeshFile, _debrisMaterial, OF_NOCOLLISION,PSHAPE_COUNT);

    if (!_explosionSoundFile.empty())
		_explosionSound=SoundSystem::getSingletonRef().loadSound(_explosionSoundFile);

	_loaded = true;
}

void ExplosionTemplate::unload()
{
	if (!_loaded)
		return;

	delete _particles;
	if (_debrisMesh)					delete _debrisMesh;
	if (_explosionSound)				SoundSystem::getSingletonRef().releaseSound(_explosionSound);

	_particles = 0;
	_debrisMesh = 0;
	_explosionEmitter = _smokeEmitter = -1;
	_explosionSound = 0;

	_loaded = false;
}
