
#pragma once

#include <string>

class Geometry;
class Material;
class Mesh;

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

	static GameMaterial*		loadMaterial(const std::string& matFile);
};
