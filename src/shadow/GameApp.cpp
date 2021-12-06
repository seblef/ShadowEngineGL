
#include "GameApp.h"
#include "../GameLib.h"
#include "../SoundLib.h"
#include "../GLMedia.h"
#include "../R2D.h"
#include "../core/YAMLCore.h"
#include "../loguru.hpp"
#include <time.h>
#include <sstream>

//#define HDAO_TUNING

#ifdef HDAO_TUNING
#include "../Renderer/HDAO.h"
#endif



void GameApp::run(const string& dataFolder)
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
	new Renderer2D(media->getVideo());

	CharacterDB *cdb=new CharacterDB("Game/characters.yaml");
	WeaponDB *wdb=new WeaponDB("Game/weapons.yaml");
	EntityDB* edb = new EntityDB("Game/entities.yaml");
	EffectDB* efdb = new EffectDB("Game/effects.yaml");

	YAML::Node map_cfg(cfg["map"]);
	string mapName(map_cfg["map"].as<string>());
	string playerName(map_cfg["player"].as<string>());

    LOG_S(INFO) << "Creating game system";
    YAML::Node game_cfg = cfg["game"];
	new GameSystem(media, mapName, playerName, game_cfg, *cdb, *wdb, *edb, *efdb);

#ifdef HDAO_TUNING
	R2D_Layer* hdao_layer = new R2D_Layer;
	R2D_Font *fnt = R2D_FontLoader::loadFont("2D/Font.sfn",media->getVideo());

	R2D_Text* hdao_normalScale = new R2D_Text(fnt);
	R2D_Text* hdao_acceptAngle = new R2D_Text(fnt);
	R2D_Text* hdao_acceptRadius = new R2D_Text(fnt);
	R2D_Text* hdao_rejectRadius = new R2D_Text(fnt);
	R2D_Text* hdao_intensity = new R2D_Text(fnt);

	hdao_normalScale->setColor(Color::White);
	hdao_normalScale->setPosition(Vector2(10.0f, 20.0f));
	hdao_normalScale->setSize(Vector2(200, 10));

	hdao_acceptAngle->setColor(Color::White);
	hdao_acceptAngle->setPosition(Vector2(10, 40));
	hdao_acceptAngle->setSize(Vector2(200, 10));

	hdao_acceptRadius->setColor(Color::White);
	hdao_acceptRadius->setPosition(Vector2(10, 60));
	hdao_acceptRadius->setSize(Vector2(200, 10));

	hdao_rejectRadius->setColor(Color::White);
	hdao_rejectRadius->setPosition(Vector2(10, 80));
	hdao_rejectRadius->setSize(Vector2(200, 10));

	hdao_intensity->setColor(Color::White);
	hdao_intensity->setPosition(Vector2(10, 100));
	hdao_intensity->setSize(Vector2(200, 10));

	hdao_layer->getSet().insert(hdao_acceptAngle);
	hdao_layer->getSet().insert(hdao_normalScale);
	hdao_layer->getSet().insert(hdao_acceptRadius);
	hdao_layer->getSet().insert(hdao_rejectRadius);
	hdao_layer->getSet().insert(hdao_intensity);

	Renderer2D::getSingletonRef().getCurrentFrame()->getVector().push_back(hdao_layer);
#endif

	bool end=false;
	while (!end)
	{
		end = GameSystem::getSingletonRef().update();

#ifdef HDAO_TUNING
		hdao_normalScale->setText("NormalScale " + to_string(HDAO::getSingletonRef().getNormalScale()));
		hdao_acceptAngle->setText("AcceptAngle " + to_string(HDAO::getSingletonRef().getAcceptAngle()));
		hdao_acceptRadius->setText("AcceptRadius " + to_string(HDAO::getSingletonRef().getAcceptRadius()));
		hdao_rejectRadius->setText("RejectRadius " + to_string(HDAO::getSingletonRef().getRejectRadius()));
		hdao_intensity->setText("Intensity " + to_string(HDAO::getSingletonRef().getIntensity()));
#endif
	}

	GameSystem::deleteSingleton();

	delete cdb;
	delete wdb;
	delete edb;

	Renderer2D::deleteSingleton();
	Renderer::deleteSingleton();
	SoundSystem::deleteSingleton();
	delete media;
}
