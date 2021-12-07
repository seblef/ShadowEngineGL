
#include "R2D_Renderer.h"
#include "../mediacommon/ITexture.h"
#include "../mediacommon/IVertexBuffer.h"
#include "../mediacommon/IIndexBuffer.h"
#include "../mediacommon/IVideoDevice.h"


R2D_Renderer::R2D_Renderer(
	R2D_Object::Type type,
	bool relativePos,
	ITexture* white,
	IVideoDevice* device
) :
	_whiteTexture(white),
	_device(device),
	_scale(2,2),
	_relativePosition(relativePos)
{
	_whiteTexture->addRef();
	if(!_relativePosition)
	{
		_scale.x/=(float)device->getResWidth();
		_scale.y/=(float)device->getResHeight();
	}

	int vxCount,indCount;
	unsigned short ind[R2D_MAX_OBJECTS*6];

	if(type==R2D_Object::R2D_LINE)
	{
		_primitiveType=PT_LINELIST;
		vxCount=R2D_MAX_OBJECTS*2;
		indCount=R2D_MAX_OBJECTS*2;

		for(int i=0;i<indCount;++i)
			ind[i]=(unsigned short)i;
	}
	else
	{
		_primitiveType=PT_TRIANGLELIST;
		vxCount=R2D_MAX_OBJECTS*4;
		indCount=R2D_MAX_OBJECTS*6;

		for(int i=0;i<R2D_MAX_OBJECTS;++i)
		{
			ind[i*6]=i*4;
			ind[i*6+1]=i*4+1;
			ind[i*6+2]=i*4+3;
			ind[i*6+3]=i*4+1;
			ind[i*6+4]=i*4+2;
			ind[i*6+5]=i*4+3;
		}
	}

	_verts=new Vx2D[vxCount];
	_vBuffer=device->createVertexBuffer(vxCount,VX_2D,BU_DYNAMIC);
	_iBuffer=device->createIndexBuffer(indCount,IDX_16BITS,BU_IMMUTABLE,ind);
}

R2D_Renderer::~R2D_Renderer()
{
	_whiteTexture->remRef();

	delete[] _verts;
	delete _vBuffer;
	delete _iBuffer;
}

void R2D_Renderer::execute()
{
	_vBuffer->set();
	_iBuffer->set();
	_device->setPrimitiveType(_primitiveType);

	sortObjects();
	renderObjects();

	_objects.clear();
}

void R2D_Renderer::makeRectVertices(const Vector2& p, const Vector2& s, const Color& c, Vx2D *v) const
{
    scaleVertex(p.x,p.y,v[0]);
    scaleVertex(p.x+s.x,p.y,v[1]);
    scaleVertex(p.x+s.x,p.y+s.y,v[2]);
    scaleVertex(p.x,p.y + s.y,v[3]);

    for(int i=0;i<4;++i)
        v[i].color=c;
}

void R2D_Renderer::makeRectVertices(const Vector2& p, const Vector2& s, const Vector2& uvStart, const Vector2& uvEnd, const Color& c, Vx2D *v) const
{
    makeRectVertices(p,s,c,v);
    v[0].uv.x=uvStart.x;					v[0].uv.y=uvStart.y;
    v[1].uv.x=uvEnd.x;						v[1].uv.y=uvStart.y;
    v[2].uv.x=uvEnd.x;						v[2].uv.y=uvEnd.y;
    v[3].uv.x=uvStart.x;					v[3].uv.y=uvEnd.y;
}

void R2D_Renderer::makeRectVertices(const R2D_Object& o, Vx2D* v) const
{
    makeRectVertices(o.getPosition(),o.getSize(),o.getColor(),v);
}

void R2D_Renderer::makeRectVertices(const R2D_Object& o, const Vector2& uvStart, const Vector2& uvEnd, Vx2D* v) const
{
    makeRectVertices(o.getPosition(),o.getSize(),uvStart,uvEnd,o.getColor(),v);
}

void R2D_Renderer::makeLineVertices(const R2D_Object& o, Vx2D *v) const
{
    scaleVertex(o.getPosition(),v[0]);
    scaleVertex(o.getPosition() + o.getSize(),v[1]);
    v[0].color=o.getColor();
    v[1].color=o.getColor();
}

void R2D_Renderer::addObject(const R2D_Object* obj)
{
    _objects.push_back(obj);
}
