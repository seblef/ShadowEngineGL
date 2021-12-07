#pragma once

#include "R2D_Renderer.h"

class R2D_Text;


class R2D_TextRenderer : public R2D_Renderer
{
protected:

	Vector2					_screenSize;

	static bool				txtSort(const R2D_Object* o1, const R2D_Object* o2);
	void					sortObjects();
	void					renderObjects();
	void					renderText(const R2D_Text& t) const;

public:

    R2D_TextRenderer(bool relativePos, ITexture* white, IVideoDevice* dev);
};
