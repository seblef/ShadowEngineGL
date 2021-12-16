#include "EditorApp.h"
#include "../editor/EditorSystem.h"
#include "../core/FileSystemFactory.h"
#include "../core/YAMLCore.h"
#include "../game/EffectDB.h"
#include "../game/EntityDB.h"
#include "../game/CharacterDB.h"
#include "../game/WeaponDB.h"
#include "../glmedia/Media.h"
#include "../renderer/Renderer.h"
#include "../sound/SoundSystem.h"
#include "../loguru.hpp"


using namespace Editor;
using namespace Core;

void initShadowCore();


void EditorApp::run(const string& dataFolder)
{
    initShadowCore();

    FileSystemFactory::createFileSystem("std", dataFolder);

	YAML::Node cfg;
	try
	{
		cfg = YAML::LoadFile("Game/config.yaml");
	}
	catch(const std::exception& e)
	{
		LOG_S(ERROR) << e.what();
		return;
	}

	YAML::Node sys_cfg(cfg["system"]);
	int width = sys_cfg["width"].as<int>(1024);
	int height = sys_cfg["height"].as<int>(768);
	bool windowed = sys_cfg["windowed"].as<bool>(true);

    IMedia *media = createGLMedia(width, height, windowed, false, SINPUT_MOUSE | SINPUT_KEYBOARD);
	new SoundSystem(media->getAudio());
	new Renderer(media->getVideo(),cfg["renderer"]);

	CharacterDB *cdb=new CharacterDB("Game/characters.yaml");
	WeaponDB *wdb=new WeaponDB("Game/weapons.yaml");
	EntityDB* edb = new EntityDB("Game/entities.yaml");
	EffectDB* efdb = new EffectDB("Game/effects.yaml");

    YAML::Node editor_cfg(cfg["editor"]);
    new EditorSystem(media, editor_cfg);

    bool end = false;
    while(!end)
    {
        end = EditorSystem::getSingletonRef().update();
    }

    EditorSystem::deleteSingleton();
	delete cdb;
	delete wdb;
	delete edb;

	Renderer::deleteSingleton();
	SoundSystem::deleteSingleton();
    delete media;
}