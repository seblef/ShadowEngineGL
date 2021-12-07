
#pragma once

#include "../core/TSingleton.h"
#include <string>

class IMedia;

class GamePlayer;
class Map;
class GameCamera;
class CharacterDB;
class WeaponDB;
class EntityDB;
class EffectDB;

namespace YAML
{
	class Node;
}

using namespace std;

class GameSystem : public Core::TSingleton<GameSystem>
{
protected:

	IMedia*				_media;
	Map*				_map;
	GamePlayer*			_player;
	GameCamera*			_camera;

	CharacterDB*		_characters;
	WeaponDB*			_weapons;
	EntityDB*			_entities;
	EffectDB*			_effects;

	double				_gameTime;
	float				_refreshTime;

public:

	GameSystem(IMedia *media, const string& mapFile, const string& playerName,
		const YAML::Node& cfg, CharacterDB& cdb, WeaponDB& wdb, EntityDB& edb, EffectDB& efdb);
	~GameSystem();

	Map&							getMap()								{ return *_map; }
	const Map&						getMap() const							{ return *_map; }

	GamePlayer&						getPlayer()								{ return *_player; }
	const GamePlayer&				getPlayer() const						{ return *_player; }

	GameCamera&						getCamera()								{ return *_camera; }
	const GameCamera&				getCamera() const						{ return *_camera; }

	CharacterDB&					getCharacterDB() const					{ return *_characters; }
	WeaponDB&						getWeaponDB() const						{ return *_weapons; }
	EntityDB&						getEntityDB() const						{ return *_entities; }
	EffectDB&						getEffectDB() const						{ return *_effects; }

	double							getGameTime() const						{ return _gameTime; }

	bool							update();
};
