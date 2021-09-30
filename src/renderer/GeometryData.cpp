
#include "GeometryData.h"

GeometryData::GeometryData(int vxCount, const Vx3D* vx, int triCount, const unsigned short *tri, IVideoDevice* d)
{
	for (int i = 0; i<vxCount; ++i)
		_bBox << vx[i].pos;

	_vBuffer = d->createVertexBuffer(vxCount, VX_3D, BU_IMMUTABLE, vx);
	_iBuffer = d->createIndexBuffer(triCount * 3, IDX_16BITS, BU_IMMUTABLE, tri);
}

GeometryData::~GeometryData()
{
	delete _iBuffer;
	delete _vBuffer;
}
