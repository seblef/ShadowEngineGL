
#ifndef _MAPTEXTLOADER_H_
#define _MAPTEXTLOADER_H_

#include "IMapLoader.h"

class MapTextLoader : public IMapLoader
{
protected:

	void					parseToken(const string& token, ScriptFile& sf, Map& m);
	void					parseGlobalLight(ScriptFile& sf);
	void					parseSound(ScriptFile& sf);
	void					parseObject(ScriptFile& sf, Map& m);
	void					parseMeshObject(ScriptFile& sf, Map& m);
	void					parseParticlesObject(ScriptFile& sf, Map& m);
	void					parseSoundObject(ScriptFile& sf, Map& m);
	void					parseLight(ScriptFile& sf, Map& m);
	void					parseActor(ScriptFile& sf, Map& m);
	void					parseEntity(ScriptFile& sf, Map& m);
	void					parseGround(ScriptFile& sf, Map& m) const;
	void					parseBuilding(ScriptFile& sf);
	void					parseBuildingObject(ScriptFile& sf, Map& m);

	ISoundCone*				parseSoundCone(ScriptFile& sf);
	void					buildMatrix(const Vector3& rot, float x, float y, float height, Matrix4& world) const
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

public:

	MapTextLoader()					{}
	~MapTextLoader()				{}

	Map*					loadMap(const string& mapName);
};

#endif