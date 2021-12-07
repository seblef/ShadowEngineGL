
#include "GameMaterial.h"
#include "Geometry.h"
#include "TextParser.h"
#include "../core/YAMLCore.h"
#include "../renderer/Mesh.h"
#include "../loguru.hpp"


GameMaterial::GameMaterial(Material *m, Geometry *d) : _rMat(m), _rDebris(0)
{
	if (d)
		_rDebris = new Mesh(d->getRGeometry(), m);
}

GameMaterial::~GameMaterial()
{

	if (_rDebris)
		delete _rDebris;
}

GameMaterial* GameMaterial::loadMaterial(const string& matFile)
{
	YAML::Node root;

	try
	{
		root = YAML::LoadFile(matFile + ".yaml");
	}
	catch(const std::exception& e)
	{
		LOG_S(ERROR) << e.what();
		return 0;
	}
    YAML::Node material = root["material"];
	return TextParser::parseMaterial(material, matFile);
}
