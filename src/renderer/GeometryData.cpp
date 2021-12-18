
#include "GeometryData.h"
#include "../core/BBox3.h"
#include "../mediacommon/IVideoDevice.h"
#include "../mediacommon/IVertexBuffer.h"
#include "../mediacommon/IIndexBuffer.h"


GeometryData::GeometryData(int vxCount, const Vx3D* vx, int triCount, const unsigned short *tri, IVideoDevice* d) :
    _bBox(Core::BBox3::InitBox)
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

void GeometryData::set()
{
    _vBuffer->set();
    _iBuffer->set();
}