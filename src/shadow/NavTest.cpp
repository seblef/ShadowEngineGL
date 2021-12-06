
#include "NavTest.h"
#include "../GameLib.h"
#include "../NavigationLib.h"
#include "../R2D.h"
#include "../GLMedia.h"
#include "../game/GamePlayer.h"
#include "../game/GameCamera.h"
#include "../core/YAMLCore.h"
#include "../loguru.hpp"
#include <time.h>
#include <sstream>


void NavTest::run(const string& dataFolder)
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
	new Renderer(media->getVideo(),cfg["renderer"]);
	Renderer2D *r2d = new Renderer2D(media->getVideo());

	R2D_Frame* r2d_frame=new R2D_Frame;
	R2D_Layer* r2d_layer=new R2D_Layer;
	R2D_Rect* r2d_start=new R2D_Rect(Vector2::NullVector,Vector2::UnitVector,Color::Blue);
	r2d_layer->getSet().insert(r2d_start);
	r2d_frame->getVector().push_back(r2d_layer);
    r2d->getFrameDB().registerData("fps",r2d_frame);
	r2d->setCurrentFrame(r2d_frame);

    CharacterDB cdb("Game/characters.yaml");
    WeaponDB wdb("Game/weapons.yaml");
	EntityDB edb("Game/entities.yaml");
	EffectDB efdb("Games/effects.yaml");

	YAML::Node map_cfg(cfg["map"]);
	string mapName(map_cfg["map"].as<string>());
	string playerName(map_cfg["player"].as<string>());
    new GameSystem(media, mapName, playerName,cfg,cdb,wdb,edb,efdb);

	new NavRenderer(media->getVideo());
	NavPath *np=NavPath::New();
	NavRenderer::getSingletonRef().setNavPath(np);
	int mx=0,my=0;

	r2d_start->setSize(Vector2(
		1.0f / (float)Navigation::getSingletonRef().getMap().getWidth(),
		1.0f / (float)Navigation::getSingletonRef().getMap().getHeight()));

	clock_t lastTime=clock();

	bool editMode=true;
	bool end=false;
	while(!end)
	{
		if(editMode)
		{
			media->update();

			Event e;
			while(EventPump::getSingletonRef().getEvent(e))
			{
				if(e._type==ET_CLOSE || (e._type==ET_KEYDOWN && e._param1==DIK_ESCAPE))
					end=true;
				else if(e._type==ET_MOUSEMOVE)
				{
					mx+=e._param1;
					my+=e._param2;

					r2d_start->setPosition(Vector2((float)mx / 800.0f,(float)my/600.0f));
				}
				else if(e._type==ET_MOUSEBUTTONDOWN)
				{
					int x,y;
					x=(int)((float)mx / 800.0f * (float)Navigation::getSingletonRef().getMap().getWidth());
					y=(int)((float)my / 600.0f * (float)Navigation::getSingletonRef().getMap().getHeight());

					Vector3 to((float)x, 0.0f, (float)y);
					Vector3 dir(GameSystem::getSingletonRef().getPlayer().getAgentDirection());
					GameSystem::getSingletonRef().getPlayer().walkTo(to,dir);

					editMode=false;
				}
			}

			media->getVideo()->clearRenderTargets(Color::Black);
			NavRenderer::getSingletonRef().renderNavMap();
			r2d->render();
			media->getVideo()->newFrame();
		}
		else
		{
			clock_t now=clock();
			float elapsed=((float)(now-lastTime)) / ((float)CLOCKS_PER_SEC);
			lastTime=now;

			Physic::getSingletonRef().update(elapsed);
			GameSystem::getSingletonRef().getCamera().update(elapsed);

			Event e;
			while(EventPump::getSingletonRef().getEvent(e))
			{
				if(e._type==ET_CLOSE || (e._type==ET_KEYDOWN && e._param1==DIK_ESCAPE))
					end=true;
				else if(e._type==ET_MOUSEMOVE)
				{
					Vector2 r(GameSystem::getSingletonRef().getCamera().getRotation());
					r.x+=(float)e._param2;
					r.y+=(float)e._param1;
					GameSystem::getSingletonRef().getCamera().setRotation(r);
				}
				else if(e._type==ET_MOUSEWHEEL)
				{
					GameSystem::getSingletonRef().getCamera().setDistance(
						GameSystem::getSingletonRef().getCamera().getDistance() * (e._param1 < 0.0f ? 1.1f : 1.0f / 1.1f));
				}
			}

			GameSystem::getSingletonRef().update();

			if(!GameSystem::getSingletonRef().getPlayer().getPath())
				editMode=true;
		}
	}
}
