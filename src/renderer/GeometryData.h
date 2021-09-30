
#ifndef _GEOMETRYDATA_H_
#define _GEOMETRYDATA_H_

#include "../MediaCommon.h"

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

	void						set()							{ _vBuffer->set(); _iBuffer->set(); }
};

#endif
