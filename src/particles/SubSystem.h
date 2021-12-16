#pragma once

#include "Material.h"
#include "../core/Color.h"
#include "../core/BBox3.h"
#include <memory>
#include <string>

namespace Core
{
class Camera;
}

namespace YAML
{
class Node;
}

namespace Particles
{

class IEmitter;
class Particle;
class ParticleRenderer;


class SubSystemParams
{
public:

    float life;
    float mass;
    float friction;

    Core::Color startColor, endColor;
    Core::Vector3 velocity;
    Core::Vector3 startSize, endSize;

    SubSystemParams()	:
        life(1.0f),
        mass(0.5f), 
        friction(0.0f),
        startColor(Core::Color::White),
        endColor(Core::Color::Black),
        velocity(Core::Vector3::YAxisVector),
        startSize(Core::Vector3::UnitVector),
        endSize(Core::Vector3::UnitVector) {}
    SubSystemParams(
        float life_,
        float mass_,
        float friction_,
        const Core::Color& startColor_,
        const Core::Color& endColor_,
        const Core::Vector3& velocity_,
        const Core::Vector3& startSize_,
        const Core::Vector3& endSize_
    ) :
        life(life_),
        mass(mass_),
        friction(friction_),
        startColor(startColor_),
        endColor(endColor_),
        velocity(velocity_),
        startSize(startSize_),
        endSize(endSize_) {}
    SubSystemParams(const SubSystemParams& p) :
        life(p.life),
        mass(p.mass),
        friction(p.friction),
        startColor(p.startColor),
        endColor(p.endColor),
        velocity(p.velocity),
        startSize(p.startSize),
        endSize(p.endSize) {}
};

class SubSystemTemplate
{
protected:
    std::string _name;
    std::string _renderer;
    std::string _emitter;
    std::unique_ptr<Material> _material;
    unsigned int _maxParticles;
    Core::Vector3 _gravity;
    float _emissionRate;
    float _lifeTime;
    SubSystemParams _minParams, _maxParams;

public:
    SubSystemTemplate(
        const std::string& name,
        const YAML::Node& node
    );
    SubSystemTemplate(
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
    );
    SubSystemTemplate(const SubSystemTemplate& t);

    const std::string& getName() const { return _name; }
    void setName(const std::string& name) { _name = name; }

    const std::string& getRenderer() const { return _renderer; }
    const std::string& getEmitter() const { return _emitter; }
    Material* getMaterial() const { return _material.get(); }

    void setRenderer(const std::string& renderer) { _renderer = renderer; }
    void setEmitter(const std::string& emitter) { _emitter = emitter; }

    const SubSystemParams& getMinParams() const { return _minParams; }
    const SubSystemParams& getMaxParams() const { return _maxParams; }

    SubSystemParams& getMinParams() { return _minParams; }
    SubSystemParams& getMaxParams() { return _maxParams; }

    unsigned int getMaxParticles() const { return _maxParticles; }
    void setMaxParticles(unsigned int maxParticles) { _maxParticles = maxParticles; }

    float getEmissionRate() const { return _emissionRate; }
    void setEmissionRate(float emissionRate) { _emissionRate = emissionRate; }

    float getLifeTime() const { return _lifeTime; }
    void setLifeTime(float lifeTime) { _lifeTime = lifeTime;}

    const Core::Vector3& getGravity() const { return _gravity; }
    void setGravity(const Core::Vector3& gravity) { _gravity = gravity; }
};

class SubSystem
{
protected:
    SubSystemTemplate* _template;
    bool _isTemplated;
    float _life;

    Core::Matrix4 _worldMatrix;
    Core::BBox3 _worldBBox;
    
    unsigned int _maxParticles;
    unsigned int _particlesCount;
    unsigned int _allocatedParticles;

    float _emissionCounter;

    Particle* _particles;
    Particle** _freeParticles;
    Particle** _usedParticles;

    IEmitter* _emitter;

    ParticleRenderer* _renderer;

    void createEmitter(const std::string& emitterClass);
    void initialize();
    void initParticle(Particle& p);
    Particle* getNextParticle();

public:
    SubSystem(SubSystemTemplate* subTemplate);
    SubSystem(
        const std::string& renderer,
        const std::string& emitter,
        Material* material,
        const SubSystemParams& minParams,
        const SubSystemParams& maxParams,
        unsigned int maxParticles,
        const Core::Vector3& gravity,
        float emissionRate,
        float lifeTime
    );
    ~SubSystem();

    void setWorldMatrix(const Core::Matrix4& worldMat);
    const Core::Matrix4& getWorldMatrix() const { return _worldMatrix; }
    const Core::BBox3& getWorldBBox() const { return _worldBBox; }

    IEmitter* getEmitter() const { return _emitter; }
    void setEmitter(const std::string& emitterClass);

    SubSystemTemplate* getTemplate() const { return _template; }

    ParticleRenderer* getRenderer() const { return _renderer; }
    void setRenderer(const std::string& renderer);

    void setMaxParticles(unsigned int maxParticles);

    void update(float time, const Core::Camera& camera);
    void collectParticles(Particle** particles, unsigned int& count, unsigned int& maxCount);

    unsigned int getParticlesCount() const { return _particlesCount; }

    void emitAll();
    void reset();
};

}