
#pragma once

#include "Effect.h"
#include "../core/Color.h"
#include <string>


class TemplateMesh;
class ISound;

namespace Particles
{
class SystemTemplate;
}

namespace YAML
{
	class Node;
}

class ExplosionTemplate : public Effect
{
protected:

	float _size;
	float _life;
	Core::Color _color;
	bool _explosionColorBlend;
	int _particleCount;

	float _smokeDensity;
	float _smokeLife;
	bool _smokeColorBlend;

	std::string _explosionTextureDir;
	std::string _smokeTexture;

	std::string _debrisMeshFile;
	std::string _debrisMaterial;
	float _debrisSize;
	float _debrisMinVelocity;
	float _debrisMaxVelocity;
	float _debrisMaxAngle;
	float _debrisMaxAngleSpeed;
	int _debrisCount;

	Particles::SystemTemplate* _particles;

	TemplateMesh* _debrisMesh;

	bool _loaded;
	int _explosionEmitter;
	int _smokeEmitter;

	std::string _explosionSoundFile;
	ISound* _explosionSound;

	bool _hitEnvironment;
	float _innerRadius;
	float _outerRadius;
	float _innerDamage;

public:

	ExplosionTemplate(const YAML::Node& node);
	~ExplosionTemplate();

	void load();
	void unload();

	Particles::SystemTemplate* getParticles() const { return _particles; }
	int getExplosionEmitter() const { return _explosionEmitter; }
	int getSmokeEmitter() const { return _smokeEmitter; }

	float getSmokeLife() const { return _smokeLife; }

	float getExplosionSize() const { return _size; }
	int getExplosionParticleCount() const { return _particleCount; }
	const Core::Color& getExplosionColor() const { return _color; }

	TemplateMesh* getDebrisMesh() const { return _debrisMesh; }
	float getDebrisSize() const { return _debrisSize; }
	float getDebrisMinVelocity() const { return _debrisMinVelocity; }
	float getDebrisMaxVelocity() const { return _debrisMaxVelocity; }
	float getDebrisMaxAngle() const { return _debrisMaxAngle; }
	float getDebrisMaxAngleSpeed() const { return _debrisMaxAngleSpeed; }
	int getDebrisCount() const { return _debrisCount; }

	ISound* getExplosionSound() const { return _explosionSound; }

	bool getHitEnvironment() const { return _hitEnvironment; }
	float getInnerRadius() const { return _innerRadius; }
	float getOuterRadius() const { return _outerRadius; }
	float getInnerDamage() const { return _innerDamage; }
};
