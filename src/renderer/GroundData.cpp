
#include "GroundData.h"


GroundData::GroundData(int width, int height, int mat_count, Material** mats, int *mat_id) : _width(width), _height(height)
{
	_mats=new Material*[mat_count];
	_triangles=new GroundTri[width*height*2];

	for(int i=0;i<mat_count;++i)
		_mats[i]=mats[i];

	GroundTri *t=_triangles;
	int* id=mat_id;
	for(int y=0;y<height;++y)
		for(int x=0;x<width;++x)
		{
			t->create(x,y,0,*id);		++t;
			t->create(x,y,1,*id);		++t;
			++id;
		}
}

GroundData::~GroundData()
{
	delete[] _mats;
	delete[] _triangles;
}


void GroundTri::create(int x, int y, int side, int mat_id)
{
    _matID=mat_id;
    for(int i=0;i<3;++i)
    {
        _verts[i].norm=Vector3::YAxisVector;
        _verts[i].tangent=Vector3::XAxisVector;
        _verts[i].binorm=Vector3::ZAxisVector;
        _verts[i].pos.y=0.0f;
    }

    float fx=(float)x;
    float fy=(float)y;

    _verts[0].pos.x=_verts[0].uv.x=fx;
    _verts[0].pos.z=_verts[0].uv.y=fy;

    if(side==0)
    {
        _verts[2].pos.x=_verts[2].uv.x=fx+1;
        _verts[2].pos.z=_verts[2].uv.y=fy+1;
        _verts[1].pos.x=_verts[1].uv.x=fx;
        _verts[1].pos.z=_verts[1].uv.y=fy+1;
    }
    else
    {
        _verts[2].pos.x=_verts[2].uv.x=fx+1;
        _verts[2].pos.z=_verts[2].uv.y=fy;
        _verts[1].pos.x=_verts[1].uv.x=fx+1;
        _verts[1].pos.z=_verts[1].uv.y=fy+1;
    }
}