
#include "GroundRenderer.h"
#include "../mediacommon/IVertexBuffer.h"
#include "../mediacommon/IShader.h"
#include <memory.h>
#include <algorithm>


#define GROUND_TRI_PER_DRAW				1024

GroundRenderer::GroundRenderer(IVideoDevice* device) : _device(device), _flagShader(0)
{
	_VBuffer=device->createVertexBuffer(GROUND_TRI_PER_DRAW*3,VX_3D,BU_DYNAMIC);
	_buffer=new Vx3D[GROUND_TRI_PER_DRAW*3];

	_bsWakable=device->createBlendState(true,BLEND_ONE,BLEND_ONE);
	_dsWakable=device->createDepthStencilState(true,false,COMP_EQUAL2);
	_rsWakable=device->createRenderState(CULL_NONE);
//	_flagShader=device->createShader("Dev/GroundFlag");
}

GroundRenderer::~GroundRenderer()
{
	delete _VBuffer;
	delete[] _buffer;
//	_flagShader->remRef();

	_device->destroyBlendState(_bsWakable);
	_device->destroyDepthStencilState(_dsWakable);
	_device->destroyRenderState(_rsWakable);
}

void GroundRenderer::visibility(const GroundData& data, const SRect& r)
{
	_triangles.clear();
	for(int x=r.x1;x<=r.x2;++x)
		for(int y=r.y1;y<=r.y2;++y)
		{
			const GroundTri& t=data.getTriangle(x,y,0);
			if(t.getMatID()!=-1)
			{
				_triangles.push_back(&t);
				_triangles.push_back(&data.getTriangle(x,y,1));
			}
		}

	sort(_triangles.begin(),_triangles.end(),sortTriangles);
}

void GroundRenderer::renderGround(const GroundData& data, bool addPass)
{
	if(_triangles.size()==0)
		return;

	_device->setInputLayer(VX_3D);
	_VBuffer->set();

    int mat_id=_triangles[0]->getMatID();
	int tri_count;
	TriVector::const_iterator tri(_triangles.begin());

	while(tri!=_triangles.end())
	{
		mat_id=(*tri)->getMatID();
		if (addPass)
		{
			if (data.getMaterial(mat_id).getFlag(MF_ADDPASS))
			{
				data.getMaterial(mat_id).setAdd(0);
				while (tri != _triangles.end() && mat_id == (*tri)->getMatID())
				{
					tri_count = 0;
					while (tri_count < GROUND_TRI_PER_DRAW &&  tri != _triangles.end() && mat_id == (*tri)->getMatID())
					{
						memcpy(_buffer + tri_count * 3, (*tri)->getVertices(), 3 * sizeof(Vx3D));
						++tri;
						++tri_count;
					}

					_VBuffer->fill(_buffer, tri_count * 3);
					_device->render(tri_count * 3);
				}
			}
			else
			{
				while (tri != _triangles.end() && mat_id == (*tri)->getMatID())
					++tri;
			}
		}
		else
		{
			data.getMaterial(mat_id).setBase(0);

			while (tri != _triangles.end() && mat_id == (*tri)->getMatID())
			{
				tri_count = 0;
				while (tri_count < GROUND_TRI_PER_DRAW &&  tri != _triangles.end() && mat_id == (*tri)->getMatID())
				{
					memcpy(_buffer + tri_count * 3, (*tri)->getVertices(), 3 * sizeof(Vx3D));
					++tri;
					++tri_count;
				}

				_VBuffer->fill(_buffer, tri_count * 3);
				_device->render(tri_count * 3);
			}
		}
	}
}

void GroundRenderer::renderFlag(unsigned int f, const unsigned int* flagMap, int w)
{
	if(_triangles.size()==0)
		return;

	_device->setInputLayer(VX_3D);
	_device->setBlendState(_bsWakable);
	_device->setDepthStencilState(_dsWakable);
	_device->setRenderState(_rsWakable);
	_VBuffer->set();
	_flagShader->set();

	int tri_count,x,y;
	TriVector::const_iterator tri(_triangles.begin());

	while(tri!=_triangles.end())
	{
		tri_count=0;
		while(tri_count < GROUND_TRI_PER_DRAW &&  tri!=_triangles.end())
		{
			x=(int)(*tri)->getVertice(0).pos.x;
			y=(int)(*tri)->getVertice(0).pos.z;

			if(flagMap[x+y*w] & f)
			{
				memcpy(_buffer + tri_count * 3, (*tri)->getVertices(), 3*sizeof(Vx3D));
				++tri_count;
			}
			++tri;
		}

		_VBuffer->fill(_buffer,tri_count*3);
		_device->render(tri_count*3);
	}
}
