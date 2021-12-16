
#include "MapTextLoader.h"
#include "Map.h"
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
#include "BuildingLight.h"
#include "BuildingMesh.h"
#include "CharacterDB.h"
#include "EntityDB.h"
#include "EntityTemplate.h"
#include "../core/YAMLCore.h"
#include "../renderer/LightSystem.h"
#include "../sound/SoundSystem.h"
#include "../loguru.hpp"



Map* MapTextLoader::loadMap(const string& mapName)
{
	Map* map = 0;

	YAML::Node root;
	try
	{
		root = YAML::LoadFile(mapName);
	}
	catch(const std::exception& e)
	{
		LOG_S(ERROR) << e.what();
		return 0;
	}

	YAML::Node map_node(root["map"]);
	int width = map_node["width"].as<int>();
	int height = map_node["height"].as<int>();
	Vector2 start_position = map_node["start_position"].as<Vector2>();

	map = new Map(width, height);
	map->setStartPosition(start_position);

	Color ambient_light = map_node["ambient_light"].as<Color>(Color::Black);
	LightSystem::getSingletonRef().getGlobalLight().setAmbientLight(ambient_light);

	YAML::Node resources(root["resources"]);

	if(root["global_light"])
		parseGlobalLight(root["global_light"]);
    
    YAML::Node materials = resources["materials"];
	if(materials)
		parseMaterials(materials);

    YAML::Node geometries = resources["geometries"];
	if(geometries)
		parseGeometries(geometries);
    
    YAML::Node meshes = resources["meshes"];
	if(meshes)
		parseMeshes(meshes);

    YAML::Node particles = resources["particles"];
	if(particles)
		parseParticles(particles);
    
    YAML::Node sounds = resources["sounds"];
	if(sounds)
		parseSounds(sounds);
    
    YAML::Node buildings = resources["buildings"];
	if(buildings)
		parseBuildings(buildings);

    YAML::Node objects = root["objects"];
	parseObjects(objects, *map);

	GroundBinLoader loader;
	loader.loadGround(map->getGround(), map_node["ground"].as<string>(), *map);

	map->finalize();
	return map;
}

void MapTextLoader::parseGlobalLight(const YAML::Node& node)
{
	Color c = node["color"].as<Color>(Color(0.9f,0.8f,0.8f,1.0f));
	Vector3 d = node["direction"].as<Vector3>(Vector3(0,-1,0));

	d.normalize();
	LightSystem::getSingletonRef().getGlobalLight().enableGlobalLight(c,d);
}

void MapTextLoader::parseMaterials(const YAML::Node& node)
{
	for(YAML::const_iterator m=node.begin(); m!=node.end(); ++m)
	{
		const string& name(m->first.as<string>());
		GameMaterial *mat = TextParser::parseMaterial(m->second, name);
		ResourceDB::getSingletonRef().getMaterialDB().registerData(name, mat);
	}
}

void MapTextLoader::parseGeometries(const YAML::Node& node)
{
	for(YAML::const_iterator g=node.begin(); g!=node.end(); ++g)
	{
		const string& name(g->as<string>());
		Geometry *geo = GeometryLoader::loadGeometry(name);
		geo->buildRGeometry();
		ResourceDB::getSingletonRef().getGeometryDB().registerData(name, geo);
	}
}

void MapTextLoader::parseMeshes(const YAML::Node& node)
{
	for(YAML::const_iterator m=node.begin(); m!=node.end(); ++m)
	{
		const string& name(m->first.as<string>());
		TemplateMesh *mesh = TextParser::parseMesh(m->second, name);
		ResourceDB::getSingletonRef().getMeshDB().registerData(name, mesh);
	}
}

void MapTextLoader::parseParticles(const YAML::Node& node)
{
	for(YAML::const_iterator p=node.begin(); p!=node.end(); ++p)
	{
		const string& name(p->first.as<string>());
		Particles::SystemTemplate *ps = TextParser::parseParticles(p->second, name);
		ResourceDB::getSingletonRef().getParticleDB().registerData(name, ps);
	}
}

void MapTextLoader::parseSounds(const YAML::Node& node)
{
	for(YAML::const_iterator s=node.begin(); s!=node.end(); ++s)
	{
		YAML::Node snd(s->second);
		const string& name(s->first.as<string>());

		int priority = snd["priority"].as<int>(1);
		float refDist = snd["reference_distance"].as<float>(2.f);
		float maxDist = snd["max_distance"].as<float>(10.f);
		float gain = snd["gain"].as<float>(1.f);
		float inAngle = snd["inner_angle"].as<float>(.0f);
		float outAngle = snd["outer_angle"].as<float>(.0f);
		bool autoPlay = snd["no_auto_play"].as<bool>(false);

		ISound* sound = SoundSystem::getSingletonRef().loadSound(snd["file"].as<string>());
		TemplateSound* tsnd = new TemplateSound(
			sound,
			maxDist, refDist,
			gain,
			inAngle, outAngle,
			priority, autoPlay
		);
		ResourceDB::getSingletonRef().getSoundDB().registerData(name, tsnd);
	}
}

void MapTextLoader::parseBuildings(const YAML::Node& node)
{
	for(YAML::const_iterator b=node.begin(); b!=node.end(); ++b)
	{
		BuildingTemplate* bt = new BuildingTemplate;
		const string& name(b->first.as<string>());
		YAML::Node build(b->second);

        YAML::Node meshes = build["meshes"];
		if(meshes)
		{
			for(YAML::const_iterator m=meshes.begin(); m!=meshes.end(); ++m)
				bt->addMesh(new BuildingMesh(m->as<string>(), 0));
		}
        YAML::Node lights;
		if(lights)
		{
			for(YAML::const_iterator l=lights.begin(); l!= lights.end(); ++l)
			{
				Light::LightType type;
				LightCreate_t lc;
				TextParser::parseLight(*l, lc, type);
				bt->addLight(new BuildingLight(type, lc, 0));
			}
		}
		ResourceDB::getSingletonRef().getBuildingDB().registerData(name, bt);
	}
}

void MapTextLoader::parseObjects(const YAML::Node& node, Map& m)
{
	for(YAML::const_iterator o=node.begin(); o!=node.end(); ++o)
	{
		const string& type((*o)["class"].as<string>());
		if(type == "mesh")
			parseMeshObject(*o, m);
		else if(type == "light")
			parseLight(*o, m);
		else if(type == "particles")
			parseParticlesObject(*o, m);
		else if(type == "sound")
			parseSoundObject(*o, m);
		else if(type == "actor")
			parseActor(*o, m);
		else if(type == "entity")
			parseEntity(*o, m);
		else if(type == "building")
			parseBuildingObject(*o, m);
		else
			LOG_S(WARNING) << "Unknown map object type " << type;
	}
}

void MapTextLoader::parseMeshObject(const YAML::Node& node, Map& m)
{
	const string& tname(node["template"].as<string>());
	TemplateMesh *tm = ResourceDB::getSingletonRef().getMeshDB().getData(tname);
	if (!tm)
	{
		LOG_S(WARNING) << "Failed getting mesh template " << tname;
		return;
	}

	Vector2 pos(node["position"].as<Vector2>());
	Vector3 rot(node["rotation"].as<Vector3>(Vector3::NullVector));
	float height(node["height"].as<float>(0.f));

	Matrix4 world;
	buildMatrix(rot, pos.x, pos.y, height == 0 ? -tm->getGeometry().getBBox().getMin().y : height, world);
	m.addObject(new GameMesh(*tm,world),false);
}

void MapTextLoader::parseActor(const YAML::Node& node, Map& m)
{
	const string& cname(node["template"].as<string>());
	Character *c = GameSystem::getSingletonRef().getCharacterDB().getData(cname);
	if (!c)
	{
		LOG_S(WARNING) << "Failed getting actor template " << cname;
		return;
	}

	Vector2 pos(node["position"].as<Vector2>());
	float orientation = node["rotation"].as<float>(0.f);

	GameCharacter* gc = new GameCharacter(*c, pos, orientation, true);
	m.addObject(gc, true);
}

void MapTextLoader::parseEntity(const YAML::Node& node, Map& m)
{
	const string& ename(node["template"].as<string>());
	EntityTemplate* et = GameSystem::getSingletonRef().getEntityDB().getData(ename);
	if (!et)
	{
		LOG_S(WARNING) << "Failed getting entity template " << ename;
		return;
	}

	Vector3 pos(node["position"].as<Vector3>());
	Vector3 rot(node["rotation"].as<Vector3>(Vector3::NullVector));

	Matrix4 world;
	buildMatrix(rot, pos.x,pos.z,pos.y,world);
	m.addObject(et->createInstance(world),true);
}

void MapTextLoader::parseParticlesObject(const YAML::Node& node, Map& m)
{
	const string& pname(node["template"].as<string>());
	Particles::SystemTemplate* tps = ResourceDB::getSingletonRef().getParticleDB().getData(pname);
	if (!tps)
	{
		LOG_S(WARNING) << "Failed getting particles template " << pname;
		return;
	}

	Vector2 pos(node["position"].as<Vector2>());
	Vector3 rot(node["rotation"].as<Vector3>(Vector3::NullVector));
	float height(node["height"].as<float>(0.f));

	Matrix4 world;
	buildMatrix(rot, pos.x, pos.y, height, world);
	m.addObject(new GameParticles(*tps, world),false);
}

void MapTextLoader::parseSoundObject(const YAML::Node& node, Map& m)
{
	const string& sname(node["template"].as<string>());
	TemplateSound* ts = ResourceDB::getSingletonRef().getSoundDB().getData(sname);
	if (!ts)
	{
		LOG_S(WARNING) << "Failed getting sound template " << sname;
		return;
	}

	Vector2 pos(node["position"].as<Vector2>());
	Vector3 rot(node["rotation"].as<Vector3>(Vector3::NullVector));
	float height(node["height"].as<float>(0.f));

	Matrix4 world;
	buildMatrix(rot, pos.x, pos.y, height, world);
	m.addObject(new GameSound(ts, world),false);

}

void MapTextLoader::parseLight(const YAML::Node& node, Map& m)
{
	Light::LightType type;
	LightCreate_t lc;
	TextParser::parseLight(node, lc, type);

	m.addObject(new GameLight(lc,type),false);
}

void MapTextLoader::parseBuildingObject(const YAML::Node& node, Map& m)
{
	const string& bname(node["template"].as<string>());
	BuildingTemplate *bt = ResourceDB::getSingletonRef().getBuildingDB().getData(bname);
	if (!bt)
	{
		LOG_S(WARNING) << "Failed getting building template " << bname;
		return;
	}

	Vector2 pos(node["position"].as<Vector2>());
	Vector3 rot(node["rotation"].as<Vector3>(Vector3::NullVector));
	float height(node["height"].as<float>(0.f));

	Matrix4 world;
	buildMatrix(rot, pos.x, pos.y, height, world);
	m.addBuilding(new Building(*bt, world, m));
}

void MapTextLoader::buildMatrix(const Core::Vector3& rot, float x, float y, float height, Core::Matrix4& world) const
{
    Matrix4 rot_x, rot_y, rot_z;
    rot_x.createRotateX(rot.x * (float)M_PI / 180.0f);
    rot_y.createRotateY(rot.y * (float)M_PI / 180.0f);
    rot_z.createRotateZ(rot.z * (float)M_PI / 180.0f);

    world = rot_z*rot_y;
    world *= rot_x;

    world(3, 0) = x;
    world(3, 1) = height;
    world(3, 2) = y;
}