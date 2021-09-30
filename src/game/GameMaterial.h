
#ifndef _GAMEMATERIAL_H_
#define _GAMEMATERIAL_H_

#include "Geometry.h"

class GameMaterial
{
protected:

	Material*					_rMat;
	Mesh*						_rDebris;

public:

	GameMaterial(Material* m, Geometry* debris);
	~GameMaterial();

	Mesh*						getDebrisMesh() const						{ return _rDebris; }
	Material*					getRMaterial() const						{ return _rMat; }

	static GameMaterial*		loadMaterial(const string& matFile);
};

#endif