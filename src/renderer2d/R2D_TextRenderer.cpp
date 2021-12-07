
#include "R2D_TextRenderer.h"
#include "R2D_Text.h"
#include "R2D_Font.h"
#include "../mediacommon/ITexture.h"
#include "../mediacommon/IVertexBuffer.h"
#include "../mediacommon/IVideoDevice.h"
#include <algorithm>


R2D_TextRenderer::R2D_TextRenderer(bool relativePos, ITexture* white, IVideoDevice* dev) : R2D_Renderer(R2D_Object::R2D_TEXT,relativePos,white,dev)
{
    _screenSize.x=(float)dev->getResWidth();
    _screenSize.y=(float)dev->getResHeight();

    _scale.x=2.0f / _screenSize.x;
    _scale.y=2.0f / _screenSize.y;
}

bool R2D_TextRenderer::txtSort(const R2D_Object* o1, const R2D_Object* o2)
{
    return ((const R2D_Text*)o1)->getFont() < ((const R2D_Text*)o2)->getFont();
}

void R2D_TextRenderer::sortObjects()
{
    sort(_objects.begin(),_objects.end(),txtSort);
}

void R2D_TextRenderer::renderObjects()
{
	ObjectVector::const_iterator o(_objects.begin());
	R2D_Font *f;
	while(o!=_objects.end())
	{
		f=((const R2D_Text*)*o)->getFont();
		f->getTexture()->set(0);

		while(o!=_objects.end() && ((const R2D_Text*)*o)->getFont()==f)
		{
			renderText(*((const R2D_Text*)*o));
			++o;
		}
	}
}

void R2D_TextRenderer::renderText(const R2D_Text& t) const
{
	Vector2 start(t.getPosition());
	if(_relativePosition)
		start*=_screenSize;

	Vector2 pos(start);
	const string& s(t.getText());
	const R2D_Font& f(*t.getFont());
	int c_count=0;
	unsigned int i=0;

	while(i < s.length())
	{
		c_count=0;
		while(i < s.length() && c_count < R2D_MAX_OBJECTS)
		{
			const R2D_FontCharArea& a(f.getCharArea(s[i]));
			if(s[i]=='\n')
			{
				pos.y+=f.getMaxHeight();
				pos.x=start.x;
			}
			else
			{
				makeRectVertices(pos,a.getSize(),a.getUVCoords().getMin(),a.getUVCoords().getMax(),t.getColor(),&_verts[c_count*4]);
				pos.x+=a.getSize().x;
			}

			++c_count;
			++i;
		}

		_vBuffer->fill(_verts,c_count*4);
		_device->renderIndexed(c_count*6);
	}
}
