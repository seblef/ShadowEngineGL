
#include "Character.h"
#include "ActorLoader.h"
#include "ActorAnimLoader.h"

Character::Character(ScriptFile& fl) : _forwardSpeed(1), _backwardSpeed(0.8f), _lateralSpeed(0.5f), _rotationSpeed(60.0f),
    _actor("Test"), _rActor(0), _rMat(0), _aiMachine("Default"), _fov((float)M_PI / 2.0f), _visDistance(30.0f),
	_lifePoints(100.0f)
{
	_name=fl.getToken();

	string t(fl.getToken());
    while(fl.good() && t!="end_character")
	{
        if(t=="actor")					_actor=fl.getToken();
        else if(t=="material")			_material=fl.getToken();
        else if(t=="forward_speed")		_forwardSpeed=stof(fl.getToken());
        else if(t=="backward_speed")	_backwardSpeed=stof(fl.getToken());
        else if(t=="lateral_speed")		_lateralSpeed=stof(fl.getToken());
        else if(t=="rotation_speed")	_rotationSpeed=stof(fl.getToken());
        else if(t=="weapon")			_weapon=fl.getToken();
		else if (t == "ai_machine")			_aiMachine = fl.getToken();
		else if (t == "ai_state")			_aiState = fl.getToken();
		else if (t == "fov")				_fov = stof(fl.getToken()) * (float)M_PI / 180.0f;
		else if (t == "visibility_dist")	_visDistance = stof(fl.getToken());
		else if (t == "life")				_lifePoints = stof(fl.getToken());
        else if(t=="animation")
		{
			_animNames.push_back(fl.getToken());
			_animFiles.push_back(fl.getToken());
		}

		t=fl.getToken();
	}
}

void Character::load()
{
	if(!_rMat && !_rActor)
	{
		_rMat=GameMaterial::loadMaterial(_material);
		_rActor=ActorLoader::loadActor(_actor,_rMat->getRMaterial(),Renderer::getSingletonRef().getVideoDevice());

		for(unsigned int i=0;i<_animFiles.size();++i)
			_rActor->addAnimation(_animNames[i],ActorAnimLoader::loadActorAnimation(_animFiles[i]));
	}
}

void Character::unload()
{
	if(_rActor)			delete _rActor;
	if(_rMat)			delete _rMat;

	_rActor=0;
	_rMat=0;
}
