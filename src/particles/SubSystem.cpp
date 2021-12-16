#include "SubSystem.h"
#include "Emitters.h"
#include "Engine.h"
#include "Material.h"
#include "Particle.h"
#include "../core/Camera.h"
#include "../core/Random.h"
#include "../core/YAMLCore.h"
#include "../loguru.hpp"


namespace Particles
{

SubSystemTemplate::SubSystemTemplate(
    const std::string& name,
    const YAML::Node& node
) :
    _name(name),
    _maxParticles(10),
    _gravity(Core::Vector3::NullVector),
    _emissionRate(0.f)
{
	if(node["life"])
	{
		_minParams.life = node["life"][0].as<float>();
		_maxParams.life = node["life"][1].as<float>();
	}
	if(node["mass"])
	{
		_minParams.mass = node["mass"][0].as<float>();
		_maxParams.mass = node["mass"][1].as<float>();
	}
	if(node["friction"])
	{
		_minParams.friction = node["friction"][0].as<float>();
		_maxParams.friction = node["friction"][1].as<float>();
	}
	if(node["velocity"])
	{
		_minParams.velocity = node["velocity"][0].as<Core::Vector3>();
		_maxParams.velocity = node["velocity"][1].as<Core::Vector3>();
	}
	if(node["start_color"])
	{
		_minParams.startColor = node["start_color"][0].as<Core::Color>();
		_maxParams.startColor = node["start_color"][1].as<Core::Color>();
	}
	if(node["end_color"])
	{
		_minParams.endColor = node["end_color"][0].as<Core::Color>();
		_maxParams.endColor = node["end_color"][1].as<Core::Color>();
	}
	if(node["start_size"])
	{
		_minParams.startSize = node["start_size"][0].as<Core::Vector3>();
		_maxParams.startSize = node["start_size"][1].as<Core::Vector3>();
	}
	if(node["end_size"])
	{
		_minParams.endSize = node["end_size"][0].as<Core::Vector3>();
		_maxParams.endSize = node["end_size"][1].as<Core::Vector3>();
	}
    YAML::Node material = node["material"];
	if(material)
		_material = std::unique_ptr<Material>(new Material(material));

	_renderer = node["renderer"].as<string>();
    _emitter = node["emitter"].as<string>();
	_maxParticles = node["max_particles"].as<int>(10);
	_gravity = node["gravity"].as<Vector3>(Vector3::NullVector);
	_emissionRate = node["emission_rate"].as<float>(0.f);
    _lifeTime = node["life_time"].as<float>(.0f);
}

SubSystemTemplate::SubSystemTemplate(
    const std::string& name,
    const std::string& renderer,
    const std::string& emitter,
    Material* material,
    const SubSystemParams& minParams,
    const SubSystemParams& maxParams,
    unsigned int maxParticles,
    const Core::Vector3& gravity,
    float emissionRate,
    float lifeTime
) :
    _name(name),
    _renderer(renderer),
    _emitter(emitter),
    _material(material),
    _minParams(minParams),
    _maxParams(maxParams),
    _maxParticles(maxParticles),
    _gravity(gravity),
    _emissionRate(emissionRate),
    _lifeTime(lifeTime)
{
}

SubSystemTemplate::SubSystemTemplate(const SubSystemTemplate& t) :
    _name(t._name),
    _renderer(t._renderer),
    _emitter(t._emitter),
    _material(new Material(*t._material)),
    _minParams(t._minParams),
    _maxParams(t._maxParams),
    _maxParticles(t._maxParticles),
    _gravity(t._gravity),
    _emissionRate(t._emissionRate),
    _lifeTime(t._lifeTime)
{
}


SubSystem::SubSystem(SubSystemTemplate* subTemplate) :
    _emitter(0),
    _template(subTemplate),
    _particlesCount(0),
    _worldMatrix(Core::Matrix4::Identity),
    _worldBBox(Core::BBox3::BoundBox),
    _emissionCounter(0.f),
    _maxParticles(subTemplate->getMaxParticles()),
    _allocatedParticles(0),
    _isTemplated(true),
    _life(.0f)
{
    initialize();
    setRenderer(subTemplate->getRenderer());
    createEmitter(subTemplate->getEmitter());
}

SubSystem::SubSystem(
    const std::string& renderer,
    const std::string& emitter,
    Material* material,
    const SubSystemParams& minParams,
    const SubSystemParams& maxParams,
    unsigned int maxParticles,
    const Core::Vector3& gravity,
    float emissionRate,
    float lifeTime
) :
    _emitter(0),
    _renderer(0),
    _maxParticles(maxParticles),
    _allocatedParticles(0),
    _isTemplated(false),
    _worldMatrix(Core::Matrix4::Identity),
    _worldBBox(Core::BBox3::BoundBox),
    _emissionCounter(0.f),
    _life(.0f)
{
    _template = new SubSystemTemplate(
        "local_template",
        renderer,
        emitter,
        material,
        minParams,
        maxParams,
        maxParticles,
        gravity,
        emissionRate,
        lifeTime
    );
    initialize();
    setRenderer(renderer);
    createEmitter(emitter);
}

SubSystem::~SubSystem()
{
    if(!_isTemplated && _template)
        delete _template;
    if(_emitter)
        delete _emitter;
    delete[] _particles;
    delete[] _freeParticles;
    delete[] _usedParticles;
}

void SubSystem::setWorldMatrix(const Core::Matrix4& worldMat)
{
    _worldMatrix = worldMat;
}

void SubSystem::setEmitter(const std::string& emitterClass)
{
    createEmitter(emitterClass);
}

void SubSystem::setRenderer(const std::string& renderer)
{
	_renderer = Engine::getSingletonRef().getRenderer(renderer);
    for(unsigned int i=0;i<_maxParticles;++i)
        _particles[i]._renderer = _renderer;
}

void SubSystem::setMaxParticles(unsigned int maxParticles)
{
    if(maxParticles <= _maxParticles)
    {
        _maxParticles = maxParticles;
        return;
    }

    _maxParticles = maxParticles;
    delete[] _particles;
    delete[] _usedParticles;
    delete[] _freeParticles;
    initialize();
}

void SubSystem::update(float time, const Core::Camera& camera)
{
    float emissionRate = _template->getEmissionRate();
    _life += time;
    if(_template->getLifeTime() > .0f && _life > _template->getLifeTime())
        emissionRate = .0f;

	_emissionCounter += time * emissionRate;
	while(_emissionCounter >= 1.0f)
	{
		_emissionCounter -= 1.0f;
		Particle *p = getNextParticle();

		if(p)
        {
            initParticle(*p);
			_emitter->emit(*p, _worldMatrix);
        }
	}

	Core::Vector3 momentum;
	const Core::Vector3& camPos(camera.getPosition());

	unsigned int currentParticle = _particlesCount;
	unsigned int putPos = 0;

	Core::Vector3 pos(_worldMatrix);
	_worldBBox.setMin(pos);
	_worldBBox.setMax(pos);

	for(unsigned int i=0;i<currentParticle;++i)
	{
		Particle *p = _usedParticles[i];

		p->_life-=time;
		if(p->_life > 0.0f)
		{
			momentum = p->_vel;
			momentum *= p->_mass;

			p->_pos += momentum * time;

			p->_color += p->_colorCounter * time;
			p->_size += p->_sizeCounter * time;
			p->_percent += p->_percentCounter * time;

			p->_vel *= 1.0f - (p->_friction * time);
			p->_vel += _template->getGravity() * time * time;

			p->_distFromCam = camPos.getDistanceFromSqr(p->_pos);

			_worldBBox << p->_pos;

			_usedParticles[putPos] = p;
			++putPos;
		}
		else
		{
			--_particlesCount;
			_freeParticles[_particlesCount] = p;
		}
	}
}

void SubSystem::collectParticles(
    Particle** particles,
    unsigned int& count,
    unsigned int& maxCount
)
{
	unsigned int c = std::min(_particlesCount, maxCount - count);
	for(unsigned int i=0;i<c;++i, ++count)
		particles[count] = _usedParticles[i];
}

void SubSystem::initialize()
{
    Material* mat = _template->getMaterial();
	_particles = new Particle[_maxParticles];
	_freeParticles = new Particle*[_maxParticles];
	_usedParticles = new Particle*[_maxParticles];

	for (unsigned int i = 0; i<_maxParticles; ++i)
	{
		_particles[i]._life = -1.0f;
		_particles[i]._renderer = _renderer;
		_particles[i]._material = mat;

		_freeParticles[i] = &_particles[i];
	}
    _particlesCount = 0;
}

void SubSystem::initParticle(Particle& p)
{
    const SubSystemParams& minParams(_template->getMinParams());
    const SubSystemParams& maxParams(_template->getMaxParams());

	p._life = Core::randomScalar(minParams.life, maxParams.life);
	p._percent = 0.f;

	if (p._life > 0.f)
		p._percentCounter = 1.0f / p._life;
	else
		p._percentCounter = 0.f;

	p._mass = Core::randomScalar(minParams.mass, maxParams.mass);
	p._friction = Core::randomScalar(minParams.friction, maxParams.friction);

	Core::randomVector(
        p._vel,
        minParams.velocity,
        maxParams.velocity
    );
	
	Core::randomColor(
        p._color,
        minParams.startColor,
        maxParams.startColor
    );
	Core::randomCounterColor(
        p._colorCounter,
        p._color,
        minParams.endColor,
        maxParams.endColor,
        p._life
    );

	Core::randomVector(
        p._size,
        minParams.startSize,
        maxParams.startSize
    );
	Core::randomCounterVector(
        p._sizeCounter,
        p._size,
        minParams.endSize,
        maxParams.endSize,
        p._life
    );
}

Particle* SubSystem::getNextParticle()
{
	Particle *p = 0;
	if(_particlesCount < _maxParticles)
	{
		p = _freeParticles[_particlesCount];
		_usedParticles[_particlesCount] = p;
		++_particlesCount;
	}
	return p;
}

void SubSystem::createEmitter(const std::string& eclass)
{
    if(_emitter)
        delete _emitter;

    if (eclass == "line")
        _emitter = new LineEmitter;
    else if(eclass == "explosion")
        _emitter = new ExplosionEmitter;
    else
        _emitter = new PointEmitter;
}

void SubSystem::emitAll()
{
    for(unsigned int i=_particlesCount; i<=_maxParticles; ++i)
    {
        Particle *p = getNextParticle();

		if(p)
        {
            initParticle(*p);
			_emitter->emit(*p, _worldMatrix);
        }
    }
}

void SubSystem::reset()
{
    _life = .0f;
    _emissionCounter = .0f;
	for (unsigned int i = 0; i<_maxParticles; ++i)
	{
		_particles[i]._life = -1.0f;
		_freeParticles[i] = &_particles[i];
	}
    _particlesCount = 0;
}

}