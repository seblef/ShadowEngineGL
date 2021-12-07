
#include "R2D_ImageRenderer.h"
#include "R2D_Image.h"
#include "../mediacommon/IVertexBuffer.h"
#include "../mediacommon/ITexture.h"
#include "../mediacommon/IVideoDevice.h"
#include <algorithm>


R2D_ImageRenderer::R2D_ImageRenderer(bool relativePos, ITexture* white, IVideoDevice* dev) :
    R2D_Renderer(R2D_Object::R2D_RECT,relativePos,white,dev)
{
}


bool R2D_ImageRenderer::imgSort(const R2D_Object* o1, const R2D_Object* o2)
{
    return ((const R2D_Image*)o1)->getTexture() < ((const R2D_Image*)o2)->getTexture();
}

void R2D_ImageRenderer::sortObjects()
{
    sort(_objects.begin(),_objects.end(),imgSort);
}

void R2D_ImageRenderer::renderObjects()
{
	ObjectVector::const_iterator o(_objects.begin());
	int o_count;
	ITexture *t;
	const R2D_Image* img;
	while(o!=_objects.end())
	{
		t=((const R2D_Image*)*o)->getTexture();
		t->set(0);

		while(o!=_objects.end() && ((const R2D_Image*)*o)->getTexture()==t)
		{
			o_count=0;
			while(o!=_objects.end() && ((const R2D_Image*)*o)->getTexture()==t && o_count < R2D_MAX_OBJECTS)
			{
				img=(const R2D_Image*)*o;
				makeRectVertices(*img,img->getUVStart(), img->getUVStart() + img->getUVSize(), &_verts[o_count*4]);

				++o;
				++o_count;
			}

			_vBuffer->fill(_verts,o_count*4);
			_device->renderIndexed(o_count*6);
		}
	}
}
