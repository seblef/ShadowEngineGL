
#include "MapTextLoader.h"
#include "TextParser.h"
#include "ResourceDB.h"
#include "GeometryLoader.h"
#include "GameMesh.h"
#include "GameParticles.h"
#include "GameLight.h"
#include "GameSound.h"
#include "GameEntity.h"
#include "GameCharacter.h"
#include "GameSystem.h"
#include "GroundBinLoader.h"
#include "Building.h"
#include "../loguru.hpp"



Map* MapTextLoader::loadMap(const string& mapName)
{
	Map* m=0;
	string token;
	ScriptFile sf(mapName);
	if(!sf.good() || sf.getToken()!="map")
    {
		LOG_S(ERROR) << "Bad map file format...";
		return m;
    }

	sf.getToken();		// Map name

	int w=stoi(sf.getToken());
	int h=stoi(sf.getToken());

	m=new Map(w,h);

	token=sf.getToken();
	while(sf.good() && token!="end_map")
	{
		parseToken(token,sf,*m);
		token=sf.getToken();
	}

	m->finalize();
	return m;
}

void MapTextLoader::parseToken(const string& token, ScriptFile& sf, Map& m)
{
	if (token == "material")
	{
		string matName;
		GameMaterial *mat=TextParser::parseMaterial(sf,&matName);
		ResourceDB::getSingletonRef().getMaterialDB().registerData(matName, mat);
	}
	else if (token == "geometry")
	{
		string geoName = sf.getToken();
		Geometry *geo = GeometryLoader::loadGeometry(geoName);
		geo->buildRGeometry();
		ResourceDB::getSingletonRef().getGeometryDB().registerData(geoName, geo);
	}
	else if(token=="global_light")
		parseGlobalLight(sf);
	else if (token == "mesh")
	{
		string meshName;
		TemplateMesh *mesh = TextParser::parseMesh(sf, &meshName);
		ResourceDB::getSingletonRef().getMeshDB().registerData(meshName, mesh);
	}
	else if (token == "particle_system")
	{
		string partName;
		TemplateParticleSystem *p = TextParser::parseParticles(sf, &partName);
		ResourceDB::getSingletonRef().getParticleDB().registerData(partName, p);
	}
	else if (token == "sound")
		parseSound(sf);
    else if(token=="object")
		parseObject(sf,m);
    else if(token=="light")
		parseLight(sf,m);
	else if (token == "ground")
		parseGround(sf, m);
	else if (token == "building")
		parseBuilding(sf);
    else if(token=="start_position")
	{
		Vector2 p;
		p.x=stof(sf.getToken());
		p.y=stof(sf.getToken());
		m.setStartPosition(p);
	}
	else if(token=="ambient_light")
	{
		Color a;
		sf.parseColor(a);
		LightSystem::getSingletonRef().getGlobalLight().setAmbientLight(a);
	}
}

void MapTextLoader::parseGlobalLight(ScriptFile& sf)
{
	Color c(0.9f,0.8f,0.8f,1.0f);
	Vector3 d(0,-1,0);

	string t=sf.getToken();
    while(sf.good() && t!="end_global_light")
	{
        if(t=="dir")
			sf.parseVector(d);
        else if(t=="color")
			sf.parseColor(c);

		t=sf.getToken();
	}

	d.normalize();
	LightSystem::getSingletonRef().getGlobalLight().enableGlobalLight(c,d);
}

void MapTextLoader::parseSound(ScriptFile& sf)
{
	string name(sf.getToken());
	string file;
	string t(sf.getToken());
	float maxDist = 10.f, refDist = 2.f;
	float gain = 1.f;
	float inAngle = .0f, outAngle = .0f;
	int priority = 1;
	bool autoPlay = true;

    while (sf.good() && t != "end_sound")
	{
		if (t == "file")
			file = sf.getToken();
		else if (t == "priority")
			priority = stoi(sf.getToken());
		else if (t == "reference_distance")
			refDist = stof(sf.getToken());
		else if(t == "max_distance")
			maxDist = stof(sf.getToken());
		else if(t == "gain")
			gain = stof(sf.getToken());
		else if(t == "inner_angle")
			inAngle = stof(sf.getToken());
		else if(t == "outer_angle")
			outAngle = stof(sf.getToken());
		else if(t == "no_auto_play")
			autoPlay = false;

		t = sf.getToken();
	}

	ISound* snd = SoundSystem::getSingletonRef().loadSound(file);
	TemplateSound* tsnd = new TemplateSound(
		snd,
		maxDist, refDist,
		gain,
		inAngle, outAngle,
		priority, autoPlay
	);

	ResourceDB::getSingletonRef().getSoundDB().registerData(name, tsnd);
}

void MapTextLoader::parseObject(ScriptFile& sf, Map& m)
{
	string t(sf.getToken());
	if (t == "mesh")
		parseMeshObject(sf, m);
	else if (t == "particles")
		parseParticlesObject(sf, m);
	else if (t == "sound")
		parseSoundObject(sf, m);
	else if (t == "actor")
		parseActor(sf, m);
	else if (t == "entity")
		parseEntity(sf, m);
	else if (t == "building")
		parseBuildingObject(sf, m);
}

void MapTextLoader::parseMeshObject(ScriptFile& sf, Map& m)
{
	TemplateMesh *tm=ResourceDB::getSingletonRef().getMeshDB().getData(sf.getToken());
	if (!tm)
	{
		LOG_S(WARNING) << "Failed getting mesh template";
		return;
	}

	float x=0,y=0,height=0;
	Vector3 rot(Vector3::NullVector);
	Matrix4 world;

	string t(sf.getToken());
	while(sf.good() && t!="end_object")
	{
		if(t=="position")
		{
			x=stof(sf.getToken());
			y=stof(sf.getToken());
		}
		else if(t=="rotation")
			sf.parseVector(rot);
		else if(t=="height")
			height=stof(sf.getToken());

		t=sf.getToken();
	}

	buildMatrix(rot, x, y, height == 0 ? -tm->getGeometry().getBBox().getMin().y : height, world);
	m.addObject(new GameMesh(*tm,world),false);
}

void MapTextLoader::parseActor(ScriptFile& sf, Map& m)
{
	Character *c = GameSystem::getSingletonRef().getCharacterDB().getData(sf.getToken());
	if (!c)
		return;

	Vector2 pos(Vector2::NullVector);
	float orientation = 0;

	string t(sf.getToken());
	while (sf.good() && t != "end_object")
	{
		if (t == "position")
		{
			pos.x = stof(sf.getToken());
			pos.y = stof(sf.getToken());
		}
		else if (t == "rotation")
			orientation = stof(sf.getToken());

		t = sf.getToken();
	}

	GameCharacter* gc = new GameCharacter(*c, pos, orientation, true);
	m.addObject(gc, true);
}

void MapTextLoader::parseEntity(ScriptFile& sf, Map& m)
{
	EntityTemplate* et = GameSystem::getSingletonRef().getEntityDB().getData(sf.getToken());
	if (!et)
		return;

	Vector3 pos(Vector3::NullVector);
	Vector3 rot(Vector3::NullVector);
	Matrix4 world;

	string t(sf.getToken());
	while (sf.good() && t != "end_object")
	{
		if (t == "position")
			sf.parseVector(pos);
		else if (t == "rotation")
			sf.parseVector(rot);

		t = sf.getToken();
	}

	buildMatrix(rot, pos.x,pos.z,pos.y,world);
	m.addObject(et->createInstance(world),true);
}

void MapTextLoader::parseParticlesObject(ScriptFile& sf, Map& m)
{
	TemplateParticleSystem* tps = ResourceDB::getSingletonRef().getParticleDB().getData(sf.getToken());
	if (!tps)
		return;

	float x = 0, y = 0, height = 0;
	Vector3 rot(Vector3::NullVector);
	Matrix4 world;

	string t(sf.getToken());
	while (sf.good() && t != "end_object")
	{
		if (t == "position")
		{
			x = stof(sf.getToken());
			y = stof(sf.getToken());
		}
		else if (t == "rotation")
			sf.parseVector(rot);
		else if (t == "height")
			height = stof(sf.getToken());

		t = sf.getToken();
	}

	buildMatrix(rot, x, y, height, world);
	m.addObject(new GameParticles(*tps, world),false);
}

void MapTextLoader::parseSoundObject(ScriptFile& sf, Map& m)
{
	TemplateSound* ts = ResourceDB::getSingletonRef().getSoundDB().getData(sf.getToken());
	if (!ts)
		return;

	float x = 0, y = 0, height = 0;
	Vector3 rot(Vector3::NullVector);
	Matrix4 world;

	string t(sf.getToken());
	while (sf.good() && t != "end_object")
	{
		if (t == "position")
		{
			x = stof(sf.getToken());
			y = stof(sf.getToken());
		}
		else if (t == "rotation")
			sf.parseVector(rot);
		else if (t == "height")
			height = stof(sf.getToken());

		t = sf.getToken();
	}

	buildMatrix(rot, x, y, height, world);
	m.addObject(new GameSound(ts, world),false);

}

void MapTextLoader::parseLight(ScriptFile& sf, Map& m)
{
	Light::LightType type;
	LightCreate_t lc;
	TextParser::parseLight(sf, lc, type);

	m.addObject(new GameLight(lc,type),false);
}

void MapTextLoader::parseGround(ScriptFile& sf, Map& m) const
{
	GroundBinLoader loader;
	loader.loadGround(m.getGround(),sf.getToken(),m);
}

void MapTextLoader::parseBuilding(ScriptFile& sf)
{
	string name(sf.getToken());
	string token(sf.getToken());
	BuildingTemplate* bt = new BuildingTemplate;

	while (sf.good() && token != "end_building")
	{
		if (token == "mesh")
			bt->addMesh(new BuildingMesh(sf.getToken(), 0));
		else if (token == "light")
		{
			Light::LightType type;
			LightCreate_t lc;
			TextParser::parseLight(sf, lc, type);
			bt->addLight(new BuildingLight(type, lc, 0));
		}

		token = sf.getToken();
	}

	ResourceDB::getSingletonRef().getBuildingDB().registerData(name, bt);
}

void MapTextLoader::parseBuildingObject(ScriptFile& sf, Map& m)
{
	BuildingTemplate *bt = ResourceDB::getSingletonRef().getBuildingDB().getData(sf.getToken());
	if (!bt)
	{
		LOG_S(WARNING) << "Failed getting building template";
		return;
	}

	float x = 0, y = 0, height = 0;
	Vector3 rot(Vector3::NullVector);
	Matrix4 world;

	string t(sf.getToken());
	while (sf.good() && t != "end_object")
	{
		if (t == "position")
		{
			x = stof(sf.getToken());
			y = stof(sf.getToken());
		}
		else if (t == "rotation")
			sf.parseVector(rot);
		else if (t == "height")
			height = stof(sf.getToken());

		t = sf.getToken();
	}

	buildMatrix(rot, x, y, height, world);
	m.addBuilding(new Building(*bt, world, m));
}
