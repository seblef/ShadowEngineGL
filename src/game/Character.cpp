
#include "Character.h"
#include "ActorLoader.h"
#include "ActorAnimLoader.h"
#include "GameMaterial.h"
#include "../renderer/Actor.h"
#include "../renderer/Renderer.h"


Character::Character(
	const string& name,
	const string& actor,
	const string& material,
	float forwardSpeed,
	float backwardSpeed,
	float lateralSpeed,
	float rotationSpeed,
	const string& weapon,
	const string& aiMachine,
	const string& aiState,
	float fov,
	float visDistance,
	float life,
	const AnimMap& anims
) :
	_name(name),
	_actor(actor),
	_material(material),
	_forwardSpeed(forwardSpeed),
	_backwardSpeed(backwardSpeed),
	_lateralSpeed(lateralSpeed),
	_rotationSpeed(rotationSpeed),
	_weapon(weapon),
	_aiMachine(aiMachine),
	_aiState(aiState),
	_fov(fov * (float)M_PI / 180.0f),
	_visDistance(visDistance),
	_lifePoints(life),
	_animations(anims),
    _rActor(0),
    _rMat(0)
{
}

void Character::load()
{
	if(!_rMat && !_rActor)
	{
		_rMat=GameMaterial::loadMaterial(_material);
		_rActor=ActorLoader::loadActor(_actor,_rMat->getRMaterial(),Renderer::getSingletonRef().getVideoDevice());

		for(auto const& anim : _animations)
			_rActor->addAnimation(anim.first, ActorAnimLoader::loadActorAnimation(anim.second));
	}
}

void Character::unload()
{
	if(_rActor)			delete _rActor;
	if(_rMat)			delete _rMat;

	_rActor=0;
	_rMat=0;
}
