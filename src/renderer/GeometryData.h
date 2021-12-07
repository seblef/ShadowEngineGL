#pragma once

#include "../core/BBox3.h"

class IVideoDevice;
class IVertexBuffer;
class IIndexBuffer;

namespace Core
{
    class Vx3D;
}

using namespace Core;

class GeometryData
{
protected:

	IVertexBuffer*				_vBuffer;
	IIndexBuffer*				_iBuffer;
	BBox3						_bBox;

public:

	GeometryData(int vxCount, const Vx3D* vx, int triCount, const unsigned short *tri, IVideoDevice* d);
	~GeometryData();

	IVertexBuffer*				getVertexBuffer() const			{ return _vBuffer; }
	IIndexBuffer*				getIndexBuffer() const			{ return _iBuffer; }
	const BBox3&				getBBox() const					{ return _bBox; }

	void						set();
};
