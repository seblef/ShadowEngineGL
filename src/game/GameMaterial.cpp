
#include "GameMaterial.h"
#include "TextParser.h"

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
	ScriptFile sf(matFile + ".txt");
	assert(sf.good());

	sf.getToken();		// material keyword
	return TextParser::parseMaterial(sf);
}
