#include "BBoxToRect.h"
#include "../core/BBox3.h"
#include "../core/BBox2.h"
#include "../core/Rect.h"
#include "../core/Vector2.h"

using namespace Core;


void computeRect(const BBox3& b, SRect& r)
{
	r.x1=(int)(floorf(b.getMin().x));			r.x2=(int)(ceilf(b.getMax().x));
	r.y1=(int)(floorf(b.getMin().z));			r.y2=(int)(ceilf(b.getMax().z));
}

void computeRect(const BBox3& b, SRect& r, const Vector2& offset)
{
	r.x1=(int)(floorf(b.getMin().x + offset.x));			r.x2=(int)(ceilf(b.getMax().x + offset.x));
	r.y1=(int)(floorf(b.getMin().z + offset.y));			r.y2=(int)(ceilf(b.getMax().z + offset.y));
}

void computeRect(const BBox2& b, SRect& r)
{
	r.x1=(int)(floorf(b.getMin().x));			r.x2=(int)(ceilf(b.getMax().x));
	r.y1=(int)(floorf(b.getMin().y));			r.y2=(int)(ceilf(b.getMax().y));
}

void computeRect(const BBox2& b, SRect& r, const Vector2& offset)
{
	r.x1=(int)(floorf(b.getMin().x + offset.x));			r.x2=(int)(ceilf(b.getMax().x + offset.x));
	r.y1=(int)(floorf(b.getMin().y + offset.y));			r.y2=(int)(ceilf(b.getMax().y + offset.y));
}
