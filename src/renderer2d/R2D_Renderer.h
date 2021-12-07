#pragma once

#include "R2D_Object.h"
#include "../mediacommon/IVertexBuffer.h"
#include <vector>

class IVideoDevice;
class IIndexBuffer;
class ITexture;

#define R2D_MAX_OBJECTS						256

class R2D_Renderer
{
protected:

	typedef std::vector<const R2D_Object*>	ObjectVector;
	ObjectVector						_objects;

	bool								_relativePosition;
    float                               _screenHeight;
	Vx2D*								_verts;
	IVideoDevice*						_device;
	IVertexBuffer*						_vBuffer;
	IIndexBuffer*						_iBuffer;
	ITexture*							_whiteTexture;
	Vector2								_scale;
	PrimitiveType						_primitiveType;

	virtual void						sortObjects()						{}
	virtual void						renderObjects()=0;

	void								scaleVertex(float x, float y, Vx2D& vout) const
	{
		vout.pos.x=x*_scale.x - 1.0f;
#ifdef R2D_NO_INVERSE
		vout.pos.y=y*_scale.y - 1.0f;
#else
        vout.pos.y=1.0f - y*_scale.y;
#endif
		vout.pos.z=0.5f;
	}
	void								scaleVertex(const Vector2& vin, Vx2D& vout) const		{ return scaleVertex(vin.x,vin.y,vout); }

	void								makeRectVertices(const Vector2& p, const Vector2& s, const Color& c, Vx2D *v) const;
	void								makeRectVertices(const Vector2& p, const Vector2& s, const Vector2& uvStart, const Vector2& uvEnd, const Color& c, Vx2D *v) const;
	void								makeRectVertices(const R2D_Object& o, Vx2D* v) const;
	void								makeRectVertices(const R2D_Object& o, const Vector2& uvStart, const Vector2& uvEnd, Vx2D* v) const;

	void								makeLineVertices(const R2D_Object& o, Vx2D *v) const;

public:

    R2D_Renderer(R2D_Object::Type type, bool relativePos, ITexture* white, IVideoDevice* device);
	virtual ~R2D_Renderer();

	void								addObject(const R2D_Object* obj);
	void								execute();
};
