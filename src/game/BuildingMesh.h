
#pragma once

#include <string>

using namespace std;

#define BMESH_INTERIOR				(1 << 0)

class BuildingMesh
{
protected:

	string			_meshName;
	unsigned int	_flags;

public:

	BuildingMesh(const string& meshName, unsigned int flags) : _meshName(meshName), _flags(flags)			{}
	~BuildingMesh()																							{}

	const string&	getMeshName() const				{ return _meshName; }
	unsigned int	getFlags() const				{ return _flags; }
};
